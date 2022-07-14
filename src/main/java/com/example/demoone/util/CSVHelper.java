package com.example.demoone.util;

import com.example.demoone.model.Flaw;
import org.apache.commons.csv.CSVFormat;

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;

public class CSVHelper {
    private static final CSVFormat csvFormat = CSVFormat.Builder.create()
            .setHeader(Flaw.HEADERS)
            .setAllowMissingColumnNames(true)
            .build();

    public static List<Flaw> processResults(List<String> results) throws IOException {
        try {
            List<Flaw> csv = csvFormat.parse(new StringReader(String.join("\n", results)))
                    .stream()
                    .filter(record -> !record.get(0).equals(Flaw.FILE))
                    .map(
                            record ->
                                    new Flaw(record.get(0), Integer.parseInt(record.get(3)),
                                            record.get(6), Integer.parseInt(record.get(1)), record.get(6), record.get(7))
                    )
                    .toList();
            return csv;
        } catch (Exception e) {
            e.printStackTrace();
            return new ArrayList<>();
        }
    }
}
