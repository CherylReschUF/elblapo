package com.example.demoone.model;

/**
 * From flawfinder man page:
 * "File" is the filename, "Line" is the line number, "Column" is the column (starting from 1),
 * "Level" is the risk level (0-5, 5 is riskiest), "Category" is the general flawfinder category,
 * "Name" is the name of the triggering rule, "Warning" is text explaining why it is a hit (finding),
 * "Suggestion" is text suggesting how it might be fixed, "Note" is other explanatory notes,
 * "CWEs" is the list of one or more CWEs,
 * "Context" is the source code line triggering the hit,
 * and "Fingerprint" is the SHA-256 hash of the context once its leading and trailing whitespace have
 * been removed (the fingerprint may help detect and eliminate later duplications). If you use Python3,
 * the hash is of the context when encoded as UTF-8.
 */

public record Flaw(String fileName, int level, String name, int line, String warning, String suggest) {
    public static final String FILE = "File";
    public static final String LEVEL = "Level";
    public static final String NAME = "Name";
    public static final String LINE = "Line";
    public static final String SUGGEST = "Suggest";
    public static final String WARNING = "Warning";
    public static final String[] HEADERS = {FILE, LINE, "Column","DefaultLevel", LEVEL, "Category", NAME, WARNING, SUGGEST};
}
