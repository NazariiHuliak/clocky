String defaultFilePath = "C:\\Users\\nazar\\Downloads\\";
String filePathInput = "";


void writeFiles() {
    String filePath = filePathInput != "" && isValidFilePath(filePathInput) ? filePathInput : defaultFilePath;
    
    long timeStamp = System.currentTimeMillis();
    
    StringBuilder colorsBuffer = new StringBuilder();
    StringBuilder keysBuffer = new StringBuilder();
    
    colorsBuffer.append("{");  
    keysBuffer.append("{");  
    
    colorMap.entrySet().stream()
      .sorted(Map.Entry.comparingByValue())
      .forEach(entry -> {
          colorsBuffer.append("{").append(entry.getKey()).append("},\n");
      });
    
    for (int i = 0; i < rows; i++) {
        keysBuffer.append("{");
        
        for (int j = 0; j < cols; j++) {
            color c = grid[i][j];
            String colorString = (int)red(c) + "," + (int)green(c) + "," + (int)blue(c);
            
            if (!colorMap.containsKey(colorString)) {
                colorMap.put(colorString, colorKeyCounter);
                colorsBuffer.append("{").append(colorString).append("},\n");
                colorKeyCounter++;
            }
            
            int key = colorMap.get(colorString);
            
            if (j == cols - 1) {
                keysBuffer.append(key);  // Last key in the row
            } else {
                keysBuffer.append(key).append(", ");
            }
        }
        
        if (i == rows - 1) {
            keysBuffer.append("}};\n");  
        } else {
            keysBuffer.append("}, \n");  
        }
    }
    
    int lastCommaIndex = colorsBuffer.lastIndexOf(",");
    if (lastCommaIndex != -1) {
        colorsBuffer.replace(lastCommaIndex, lastCommaIndex + 1, "};\n");
    }
    
    PrintWriter colorsFile = createWriter(filePath + "colors" + str(timeStamp) + ".txt");
    colorsFile.print(colorsBuffer.toString());
    
    PrintWriter keysFile = createWriter(filePath + "keys" + str(timeStamp) + ".txt");
    keysFile.print(keysBuffer.toString());
    
    colorsFile.close();
    keysFile.close();

    println("Files written: colors.txt and keys.txt to " + filePath);
}

boolean isValidFilePath(String filePath) {
    if (filePath == null || filePath.trim().isEmpty()) {
        println("File path is null or empty.");
        return false;
    }

    String invalidCharacters = "[<>\"|?*]";
    if (filePath.matches(".*" + invalidCharacters + ".*")) {
        println("File path contains invalid characters: " + invalidCharacters);
        return false;
    }

    if (filePath.length() > 260) {
        println("File path exceeds maximum allowed length (260 characters).");
        return false;
    }

    return true;
}
