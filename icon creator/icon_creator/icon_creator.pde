import java.util.*;
import java.util.stream.*;

// matrix
int rows = 8;
int cols = 7;
int cellSize = 30;  
int startSpace = 10;


// colors
color[][] grid = new color[rows][cols];
//color selectedColor = color(255, 255, 255);
int[] selectedColorValues = new int[]{120, 255, 255};
color selectedColor = color(selectedColorValues[0], selectedColorValues[1], selectedColorValues[2]);
int minValue = 0;
int maxValue = 255; 


// pallete 
int paletteX = cols * 30 + 20;
int paletteY = 10;
int colorBoxY = paletteY + 100;
int colorBoxX = paletteX;
int colorBoxHeight = 30;
int colorBoxWidth = 50;


//input fields
String[] inputs = {str(selectedColorValues[0]), str(selectedColorValues[1]), str(selectedColorValues[2])};
boolean[] isFocused = {false, false, false};
boolean[] wasCleared = {false, false, false};
int fieldX = paletteX + 320;
int fieldY = paletteY;
int fieldWidth = 30, fieldHeight = 20;
int fieldSpacing = 6;


// sliders
int sliderX = paletteX + 110; 
int sliderY = paletteY; 
int sliderWidth = 200; 
int sliderHeight = 10; 
int sliderSpacing = 30;

int[] sliderValues = new int[]{selectedColorValues[0], selectedColorValues[1], selectedColorValues[2]};

//buttons 
int buttonX = paletteX;
int buttonY = colorBoxY + colorBoxHeight + 50; 
int buttonHeight = 20;
int buttonWidth = 80;


//File writer
HashMap<String, Integer> colorMap = new HashMap<String, Integer>();
int colorKeyCounter = 0;

boolean isFileInputFocused = false;
int fileInputX = paletteX + 200;
int fileInputY = buttonY;
int fieldPathInputWidth = 100;


void setup() {
  size(590, 340);  

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      grid[i][j] = color(0); 
    }
  }
  
  
  int index = 0;
    for (int r = 0; r <= 255; r += 51) { // Step size of 51 for variety
        for (int g = 0; g <= 255; g += 51) {
            for (int b = 0; b <= 255; b += 51) {
                String colorKey = r + "," + g + "," + b;
                colorMap.put(colorKey, index++);
            }
        }
    }
  stroke(0,0,0);
}

void draw() {
  background(220);
  stroke(255);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      fill(grid[i][j]);
      stroke(1);
      rect(startSpace + j * cellSize, startSpace + i * cellSize, cellSize - 1, cellSize - 1);
    }
  }

  drawColorPalette();
  drawButtonsPanel();
  drawSliderPanel();
  drawScrollableColorPalette();
}



void processMatrixBoxClick() {
  if(mouseButton == RIGHT) return;
  
  if (mouseY < rows * cellSize) {
    int row = (mouseY - startSpace) / cellSize;
    int col = (mouseX - startSpace) / cellSize;
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
      if (grid[row][col] == selectedColor) {
        grid[row][col] = color(0, 0, 0);
      } else {
        grid[row][col] = selectedColor;  
      }
    }
  }
}

void processColorPicker() {
  if (mouseButton == RIGHT) {
    if (mouseY < rows * cellSize) {
    int row = (mouseY - startSpace) / cellSize;
    int col = (mouseX - startSpace) / cellSize;
      if (row >= 0 && row < rows && col >= 0 && col < cols) {
        selectedColor = grid[row][col];
        updateColor(selectedColor);
      }
    }
  }
}

void importColors(String data) {
  HashMap<String, Integer> importColorMap = new HashMap<>();
  String[] lines = data.split("\\n");

  int index = 0;

  for (String line : lines) {
    line = line.replaceAll("[{};\\s]+", "").trim();
    if (line.matches("\\d+,\\d+,\\d+,?")) {
      if(line.endsWith(",")) {
        line = line.substring(0, line.length() - 1);
      }

      importColorMap.put(line, index);
      index++;
    }
  }

  if (importColorMap.size() != 0) {
    colorMap = importColorMap;
    colorKeyCounter = index;
    println(colorKeyCounter);
    totalWidth = colorMap.size() * rectWidth;
  }
}

void importIcon(String data) {
  if(colorMap.isEmpty()) {
    System.out.println("Colors do not exist");
    return;
  }
  
  String[] lines = data.split("\\n");
  var colorMapReversed = colorMap.entrySet().stream()
                            .collect(Collectors.toMap(Map.Entry::getValue, Map.Entry::getKey));

  if (lines.length != rows) {
      println("Error: The imported icon has an invalid size. Expected " + rows + " rows, but found " + lines.length + " rows. Please check the input.");
      return;
  }

  for (int i = 0; i < lines.length; i++) {
    String line = lines[i].replaceAll("[{};\\s]+", "").trim();
    String[] colorIndexes = line.split(",");

    if (colorIndexes.length != cols) {
      println("Error: The imported icon has an invalid size. Expected " + cols + " columns, but found " + colorIndexes.length + " columns. Please verify the input.");
      return;
    }

    for (int j = 0; j < cols; j++) {
      try {
        int currentColorIndex = Integer.parseInt(colorIndexes[j]);
        String[] colorComponentStr = colorMapReversed.get(currentColorIndex).split(",");
        
        if (colorComponentStr.length != 3) {
            throw new IllegalArgumentException("Invalid color format: expected 3 components (R, G, B), but got " + colorComponentStr.length);
        }
        
        grid[i][j] = color(Integer.parseInt(colorComponentStr[0]), Integer.parseInt(colorComponentStr[1]), Integer.parseInt(colorComponentStr[2]));
      } catch (NumberFormatException e) {
          System.out.println("Error: Invalid number format while parsing color index at position [" + i + "][" + j + "]. Please check the input.");
      } catch (IllegalArgumentException e) {
          System.out.println("Error: " + e.getMessage() + " at position [" + i + "][" + j + "].");
      } catch (Exception e) {
          System.out.println("An unexpected error occurred during importing the icon at position [" + i + "][" + j + "].");
      }
    }
  }
}

void mousePressed() {
  processMatrixBoxClick();
  processColorPicker();
  processSliderInputFieldsClick();
  processFilePathInputFieldClick();
  processColorPaletteClick();
  processButtonsClick();
  processScrollablePaletteClick();
}

void keyPressed() {
  processSliderInputFieldsKeyPressed();
  processFilePathInputFieldKeyPressed();
}

void mouseDragged() {
  processColorSliderMouseDragged();
  processScrollablePaletteMouseDragged();
}

void mouseReleased() {
    dragging = false;
}
