package com.daoekinc.pinfit;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.fail;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Comparator;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;
import java.util.stream.Stream;
import org.junit.jupiter.api.io.TempDir;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;

/**
 * Freezes the generated C. Each fixture under {@code src/test/resources/golden/<name>} holds an
 * {@code input} project and the exact files {@code generate} must produce from it, byte for byte
 * (line endings included - the directory is {@code -text} in .gitattributes).
 *
 * <p>A failure here means a change alters what users get on their next regeneration. If that is
 * intended, re-baseline with {@code mvn test -Dtest=GoldenOutputTest -Dpinfit.golden.update=true}
 * and review the diff of {@code expected/} like any other change. CI also compiles these expected
 * files with gcc (see scripts/ci/compile-golden.sh).
 */
class GoldenOutputTest {
    private static final Path GOLDEN = Path.of("src", "test", "resources", "golden");

    @TempDir
    Path temporaryDirectory;

    @ParameterizedTest(name = "{0}")
    @ValueSource(strings = {"default", "variant"})
    void generatedOutputMatchesGoldenFiles(String fixture) throws IOException {
        Path input = GOLDEN.resolve(fixture).resolve("input");
        Path expected = GOLDEN.resolve(fixture).resolve("expected");
        copyTree(input, temporaryDirectory);

        CliFixture cli = new CliFixture(temporaryDirectory);
        assertEquals(0, cli.run("generate"), cli.errors());

        Set<String> inputFiles = relativeFiles(input);
        Set<String> generated = new TreeSet<>(relativeFiles(temporaryDirectory));
        generated.removeAll(inputFiles);

        if (Boolean.getBoolean("pinfit.golden.update")) {
            deleteTree(expected);
            for (String file : generated) {
                Path target = expected.resolve(file);
                Files.createDirectories(target.getParent());
                Files.copy(temporaryDirectory.resolve(file), target);
            }
            return;
        }

        assertEquals(relativeFiles(expected), generated, "set of generated files changed for fixture '" + fixture + "'");
        for (String file : generated) {
            byte[] actual = Files.readAllBytes(temporaryDirectory.resolve(file));
            byte[] wanted = Files.readAllBytes(expected.resolve(file));
            if (!java.util.Arrays.equals(actual, wanted)) {
                fail(fixture + "/" + file + " differs from its golden copy: " + firstDifference(wanted, actual)
                        + "\nIf intended, re-run with -Dpinfit.golden.update=true and review the expected/ diff.");
            }
        }
    }

    private static String firstDifference(byte[] wanted, byte[] actual) {
        List<String> a = List.of(new String(wanted, StandardCharsets.UTF_8).split("\n", -1));
        List<String> b = List.of(new String(actual, StandardCharsets.UTF_8).split("\n", -1));
        for (int line = 0; line < Math.max(a.size(), b.size()); line++) {
            String left = line < a.size() ? a.get(line) : "<end of file>";
            String right = line < b.size() ? b.get(line) : "<end of file>";
            if (!left.equals(right)) {
                return "line " + (line + 1) + "\n  expected: " + left.replace("\r", "\\r")
                        + "\n  actual:   " + right.replace("\r", "\\r");
            }
        }
        return "same text, different bytes";
    }

    private static Set<String> relativeFiles(Path root) throws IOException {
        Set<String> files = new TreeSet<>();
        if (!Files.isDirectory(root)) {
            return files;
        }
        try (Stream<Path> paths = Files.walk(root)) {
            paths.filter(Files::isRegularFile)
                    .forEach(path -> files.add(root.relativize(path).toString().replace('\\', '/')));
        }
        return files;
    }

    private static void copyTree(Path from, Path to) throws IOException {
        try (Stream<Path> paths = Files.walk(from)) {
            for (Path source : paths.toList()) {
                Path target = to.resolve(from.relativize(source).toString());
                if (Files.isDirectory(source)) {
                    Files.createDirectories(target);
                } else {
                    Files.copy(source, target);
                }
            }
        }
    }

    private static void deleteTree(Path root) throws IOException {
        if (!Files.exists(root)) {
            return;
        }
        try (Stream<Path> paths = Files.walk(root)) {
            for (Path path : paths.sorted(Comparator.reverseOrder()).toList()) {
                Files.delete(path);
            }
        }
    }
}
