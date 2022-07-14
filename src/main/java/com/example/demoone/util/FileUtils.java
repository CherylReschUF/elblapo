package com.example.demoone.util;

import java.io.*;
import java.util.Arrays;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

public class FileUtils {
    public static void saveStudentFiles(File selectedFile, String csvContents, String htmlContents, String filename) {
        try {
            File csvOutput = new File(selectedFile.getAbsolutePath(), filename + ".csv");
            FileWriter fw = new FileWriter(csvOutput);
            fw.write(csvContents);
            fw.close();
            File htmlOutput = new File(selectedFile.getAbsolutePath(), filename + ".html");
            fw = new FileWriter(htmlOutput);
            fw.write(htmlContents);
            fw.close();
            zipFiles(csvOutput,htmlOutput);
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
    public static void zipFiles(File... files) throws IOException {
        List<String> srcFiles = Arrays.stream(files).map(File::getAbsolutePath).toList();
        var zipFileName = srcFiles.get(0).split("[.]")[0];
        FileOutputStream fos = new FileOutputStream(zipFileName + ".zip");
        ZipOutputStream zipOut = new ZipOutputStream(fos);
        for (String srcFile : srcFiles) {
            File fileToZip = new File(srcFile);
            FileInputStream fis = new FileInputStream(fileToZip);
            ZipEntry zipEntry = new ZipEntry(fileToZip.getName());
            zipOut.putNextEntry(zipEntry);
            byte[] bytes = new byte[1024];
            int length;
            while((length = fis.read(bytes)) >= 0) {
                zipOut.write(bytes, 0, length);
            }
            fis.close();
            fileToZip.delete();
        }
        zipOut.close();
        fos.close();
    }
}
