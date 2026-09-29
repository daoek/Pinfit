package com.daoekinc.pinfit;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.fail;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Stream;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

/**
 * Keeps the documentation's generated-output examples honest. For each page, the YAML blocks it
 * shows are written into a fresh project and generated; every listed C block on the page must then
 * appear in the real output - its lines in order, other lines allowed in between, so a block may be
 * an excerpt. A failure means the docs no longer show what Pinfit produces: fix the page, or the
 * generator.
 */
class DocExamplesTest {
    private static final Pattern TITLED_BLOCK = Pattern.compile(
            "```(\\w+) title=\"([^\"]+)\"[^\\n]*\\n(.*?)```", Pattern.DOTALL);

    // Interfaces some pages reference but do not show in a titled block of their own.
    private static final String COMMON_IIC = """
            kind: interface
            name: common_iic
            invalidReturn: COMMON_IIC_INVALID_PARAM
            uninitializedReturn: COMMON_IIC_NOT_INITIALIZED
            includes: [<stdint.h>]
            enums:
              - name: common_iic_status_t
                values:
                  - { name: COMMON_IIC_SUCCESS, value: 0 }
                  - { name: COMMON_IIC_INVALID_PARAM, value: 1 }
                  - { name: COMMON_IIC_NOT_INITIALIZED, value: 2 }
            functions:
              - name: write
                return: common_iic_status_t
                parameters: [uint32_t slave_address, const uint8_t *data, uint32_t length]
            """;
    private static final String BUTTON_LISTENER = """
            kind: interface
            name: button_listener
            includes: [<stdint.h>]
            functions:
              - { name: pressed, return: void, parameters: [uint8_t button_id] }
            """;
    private static final String BUS = """
            kind: interface
            name: bus
            includes: [<stdint.h>]
            invalidReturn: -1
            uninitializedReturn: -2
            functions:
              - { name: write, return: int, parameters: [const uint8_t *data, uint32_t length] }
            """;
    private static final String BUS_HAL = BUS.replace("name: bus\n", "name: bus_hal\n").replace("name: write", "name: send");

    /**
     * @param page       docs-relative page
     * @param extraFiles specs the page relies on without showing them
     * @param checked    doc block title -> generated file it must match (first block with that title)
     */
    private record Page(String page, Map<String, String> extraFiles, Map<String, String> checked) {
    }

    static Stream<Arguments> pages() {
        return Stream.of(
                new Page("getting-started/quickstart.md", Map.of(), Map.of(
                        "pinfit.yaml", "pinfit.yaml",
                        "drivers/Interface/common_iic_I.h", "drivers/Interface/common_iic_I.h",
                        "drivers/RA/ra_iic.h", "drivers/RA/ra_iic.h",
                        "drivers/RA/ra_iic.c", "drivers/RA/ra_iic.c")),
                new Page("generators/interface.md", Map.of(), Map.of(
                        "drivers/Interface/common_iic_I.h", "drivers/Interface/common_iic_I.h")),
                new Page("generators/module.md", Map.of("drivers/Interface/common_iic.interface.yaml", COMMON_IIC), Map.of(
                        "ra_iic.c", "drivers/RA/ra_iic.c")),
                new Page("generators/state-machine.md", Map.of(), Map.of(
                        "door.h", "door.h",
                        "door.c", "door.c")),
                new Page("generators/observer.md", Map.of("button_listener.interface.yaml", BUTTON_LISTENER), Map.of(
                        "button_events.h", "button_events.h")),
                new Page("generators/command-table.md", Map.of(), Map.of(
                        "uart_cmd.h", "uart_cmd.h",
                        "uart_cmd.c", "uart_cmd.c")),
                new Page("generators/status-codes.md", Map.of(), Map.of(
                        "pinfit_status.h", "pinfit_status.h")),
                new Page("generators/adapter.md", Map.of("bus.interface.yaml", BUS, "bus_hal.interface.yaml", BUS_HAL), Map.of(
                        "bus_adapter.h", "bus_adapter.h",
                        "bus_adapter.c", "bus_adapter.c"))
        ).map(page -> Arguments.of(page.page(), page));
    }

    @TempDir
    Path temporaryDirectory;

    @ParameterizedTest(name = "{0}")
    @MethodSource("pages")
    void generatedOutputShownOnPageIsReal(String name, Page page) throws IOException {
        String doc = Files.readString(Path.of("docs").resolve(page.page())).replace("\r\n", "\n");
        // Named like the docs' example project, so pinfit.yaml's generated name matches too.
        Path project = temporaryDirectory.resolve("firmware");
        Files.createDirectories(project);
        CliFixture cli = new CliFixture(project);
        assertEquals(0, cli.run("init"), cli.errors());

        List<Block> blocks = blocks(doc);
        List<String> written = new ArrayList<>();
        for (Block block : blocks) {
            // A later block for the same spec is a follow-up edit (e.g. "now add a second function").
            if (block.language().equals("yaml") && !block.title().equals("pinfit.yaml") && !written.contains(block.title())) {
                write(project.resolve(block.title()), block.body());
                written.add(block.title());
            }
        }
        for (Map.Entry<String, String> extra : page.extraFiles().entrySet()) {
            write(project.resolve(extra.getKey()), extra.getValue());
        }
        assertEquals(0, cli.run("generate"), cli.errors());

        for (Map.Entry<String, String> check : page.checked().entrySet()) {
            Block block = blocks.stream().filter(candidate -> candidate.title().equals(check.getKey())).findFirst()
                    .orElseThrow(() -> new AssertionError(page.page() + " has no block titled '" + check.getKey() + "'"));
            Path generated = project.resolve(check.getValue());
            assertTrue(Files.isRegularFile(generated), page.page() + ": nothing generated at " + check.getValue());
            assertOrderedSubset(page.page(), check.getKey(), block.body(), Files.readString(generated));
        }
    }

    private record Block(String language, String title, String body) {
    }

    private static List<Block> blocks(String doc) {
        List<Block> blocks = new ArrayList<>();
        Matcher matcher = TITLED_BLOCK.matcher(doc);
        while (matcher.find()) {
            blocks.add(new Block(matcher.group(1), matcher.group(2), matcher.group(3)));
        }
        return blocks;
    }

    private static void assertOrderedSubset(String page, String title, String docBody, String actual) {
        List<String> real = actual.replace("\r\n", "\n").lines().filter(line -> !line.contains("skeleton-hash")).toList();
        int position = 0;
        for (String line : docBody.stripTrailing().lines().toList()) {
            int found = -1;
            for (int index = position; index < real.size(); index++) {
                if (real.get(index).equals(line)) {
                    found = index;
                    break;
                }
            }
            if (found < 0) {
                fail(page + ", block '" + title + "': this line does not appear (in order) in the generated file:\n  "
                        + line + "\n--- generated file ---\n" + String.join("\n", real));
            }
            position = found + 1;
        }
    }

    private static void write(Path path, String content) throws IOException {
        Files.createDirectories(path.getParent());
        Files.writeString(path, content);
    }
}
