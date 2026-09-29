package com.daoekinc.pinfit.statesmith;

import com.daoekinc.pinfit.PinfitException;
import com.daoekinc.pinfit.model.ProjectConfig;
import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.concurrent.TimeUnit;

/**
 * Invokes StateSmith's {@code ss.cli} as an external process. Pinfit never bundles or downloads it.
 *
 * <p>Verified against a real {@code ss.cli 0.22.2} install: its exit code is 0 even on total
 * failure (missing file, invalid TOML, invalid UML) - the only reliable success signal is the
 * text {@code StateSmith Runner - Finished normally.} in its combined stdout/stderr. This class
 * checks that text, never the exit code.
 */
public final class StateSmithRunner {
    private static final String SUCCESS_MARKER = "Finished normally.";
    /** A normal run takes seconds; anything this long is a tool stuck on something, not working. */
    private static final long TIMEOUT_SECONDS = 180;

    /**
     * Runs {@code <command> --version} and checks the configured {@code stateSmith.version} is a
     * substring of the output (StateSmith prints e.g. {@code StateSmith.Cli 0.22.2+<hash>}, so an
     * exact match would break on every rebuild hash). Throws with install/version instructions on
     * a missing tool or a mismatch.
     */
    public void checkVersion(ProjectConfig project) {
        ProjectConfig.StateSmithSettings settings = project.stateSmith();
        if (settings.version() == null || settings.version().isBlank()) {
            throw new PinfitException("pinfit.yaml is missing stateSmith.version, required because this project "
                    + "has an 'engine: statesmith' state machine.",
                    "Add to pinfit.yaml", "stateSmith:\n  command: " + settings.command() + "\n  version: 0.22.2");
        }
        ProcessResult result = execute(settings.command(), List.of("--version"), null);
        if (result == null) {
            throw new PinfitException("Cannot run '" + settings.command() + "' - is StateSmith's CLI installed and on PATH?",
                    "Install StateSmith.Cli", "https://github.com/StateSmith/StateSmith/wiki/CLI:-download-or-install");
        }
        if (!result.output().contains(settings.version())) {
            throw new PinfitException("Installed StateSmith CLI does not report version '" + settings.version()
                    + "' (configured in pinfit.yaml's stateSmith.version):\n\n" + result.output().strip(),
                    "Fix it by", "installing StateSmith.Cli " + settings.version()
                            + ", or updating pinfit.yaml's stateSmith.version to match what's installed.");
        }
    }

    /**
     * Runs {@code ss.cli run <plantumlFile>} with the plantuml file's own directory (folder B) as
     * the working directory, so StateSmith's default next-to-the-input output naming writes
     * {@code <name>_sm.c/.h} (and its simulator {@code .html}) there too.
     */
    public void run(Path plantumlFile, Path yamlFile, ProjectConfig project) {
        ProjectConfig.StateSmithSettings settings = project.stateSmith();
        Path directory = plantumlFile.getParent();
        String fileArgument = plantumlFile.getFileName().toString();
        ProcessResult result = execute(settings.command(), List.of("run", fileArgument, "--no-ask"), directory);
        if (result == null) {
            throw new PinfitException("Cannot run '" + settings.command() + "' for " + yamlFile
                    + " - is StateSmith's CLI installed and on PATH?");
        }
        if (!result.output().contains(SUCCESS_MARKER)) {
            throw new PinfitException("StateSmith failed generating from " + yamlFile + ":\n\n" + result.output().strip());
        }
    }

    private static ProcessResult execute(String command, List<String> arguments, Path workingDirectory) {
        ProcessResult result = start(command, arguments, workingDirectory);
        // Confirmed against a real install: ss.cli's own executable is literally "ss.cli.exe"
        // (the command name itself already contains a dot, so this can't check for "no dot"),
        // and Windows' CreateProcess (unlike a shell) does not append .exe for a bare command
        // name even when its directory is on PATH. Java's ProcessBuilder inherits that behavior,
        // so a plain `command: ss.cli` in pinfit.yaml (the documented default) would otherwise
        // never resolve. Retry once with .exe appended before giving up.
        if (result == null && !command.toLowerCase(Locale.ROOT).endsWith(".exe")) {
            result = start(command + ".exe", arguments, workingDirectory);
        }
        return result;
    }

    private static ProcessResult start(String command, List<String> arguments, Path workingDirectory) {
        List<String> commandLine = new ArrayList<>();
        commandLine.add(command);
        commandLine.addAll(arguments);
        Path outputFile;
        try {
            outputFile = Files.createTempFile("pinfit-statesmith-", ".log");
        } catch (IOException exception) {
            throw new PinfitException("Cannot create a temporary file for StateSmith's output: " + exception.getMessage(), exception);
        }
        // stdin from the null device: a tool that unexpectedly prompts gets end-of-file instead of
        // waiting forever on a pipe nobody writes to. Output goes to a file rather than a pipe so the
        // time limit below can be enforced while the tool runs.
        ProcessBuilder builder = new ProcessBuilder(commandLine)
                .redirectInput(ProcessBuilder.Redirect.from(nullDevice()))
                .redirectErrorStream(true)
                .redirectOutput(outputFile.toFile());
        if (workingDirectory != null) {
            builder.directory(workingDirectory.toFile());
        }
        try {
            Process process;
            try {
                process = builder.start();
            } catch (IOException exception) {
                return null;
            }
            if (!process.waitFor(TIMEOUT_SECONDS, TimeUnit.SECONDS)) {
                process.destroyForcibly();
                throw new PinfitException("'" + String.join(" ", commandLine) + "' did not finish within "
                        + TIMEOUT_SECONDS + " seconds and was stopped.",
                        "Fix it by", "running that command yourself in " + (workingDirectory == null ? "this directory" : workingDirectory)
                                + " to see what it is waiting for, then generate again.");
            }
            // Lenient decode: console tools do not always emit valid UTF-8.
            return new ProcessResult(new String(Files.readAllBytes(outputFile), StandardCharsets.UTF_8));
        } catch (IOException exception) {
            throw new PinfitException("Cannot read StateSmith's output: " + exception.getMessage(), exception);
        } catch (InterruptedException exception) {
            Thread.currentThread().interrupt();
            throw new PinfitException("Interrupted while running '" + command + "'", exception);
        } finally {
            try {
                Files.deleteIfExists(outputFile);
            } catch (IOException ignored) {
                // A leftover temp log is harmless.
            }
        }
    }

    private static File nullDevice() {
        return new File(System.getProperty("os.name").toLowerCase(Locale.ROOT).startsWith("windows") ? "NUL" : "/dev/null");
    }

    private record ProcessResult(String output) {
    }
}
