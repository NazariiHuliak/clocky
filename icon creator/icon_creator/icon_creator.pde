// matrix
int rows = 8;
int cols = 7;
int cellSize = 30;  
int startSpace = 10;


// colors
color[][] grid = new color[rows][cols];
//boolean customColorWasChosen = false;
color selectedColor = color(255, 255, 255);
int[] customColorValues = new int[]{120, 255, 255};
color customColor = color(customColorValues[0], customColorValues[1], customColorValues[2]);
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
String[] inputs = {str(customColorValues[0]), str(customColorValues[1]), str(customColorValues[2])};
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

int sliderValue1 = customColorValues[0]; 
int sliderValue2 = customColorValues[1]; 
int sliderValue3 = customColorValues[2];

//buttons 
int buttonX = paletteX;
int buttonY = colorBoxY + colorBoxHeight + 50; 
int buttonHeight = 20;
int buttonWidth = 60;

HashMap<color, Integer> colorMap = new HashMap<color, Integer>();
int colorKeyCounter = 1;
void setup() {
  size(8 * 30 + 350, 8 * 30 + 100);  

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      grid[i][j] = color(0); 
    }
  }
  
  stroke(0,0,0);
}

void draw() {
  background(220);
  stroke(255);//
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      fill(grid[i][j]);
      stroke(1);
      rect(startSpace + j * cellSize, startSpace + i * cellSize, cellSize - 1, cellSize - 1);
    }
  }

  drawColorPalette();
  drawControlPanel();
}

void drawColorPalette() {
  fill(255, 0, 0);  // Red color
  rect(colorBoxX, colorBoxY, colorBoxWidth, colorBoxHeight);

  fill(0, 255, 0);  // Green color
  rect(colorBoxX + 60, colorBoxY, colorBoxWidth, colorBoxHeight);

  fill(0, 0, 255);  // Blue color
  rect(colorBoxX + 120, colorBoxY, colorBoxWidth, colorBoxHeight);

  fill(255, 255, 0);  // Yellow color
  rect(colorBoxX + 180, colorBoxY, colorBoxWidth, colorBoxHeight);
  
  fill(255, 255, 255);  // white color
  rect(colorBoxX + 240, colorBoxY, colorBoxWidth, colorBoxHeight);
  
  fill(100, 100, 100);  // grey color
  rect(colorBoxX + 300, colorBoxY, colorBoxWidth, colorBoxHeight);
  
  drawSliderPanel();
}

void drawControlPanel() {
   
   fill(100, 100, 100);  // grey color
   rect(buttonX, buttonY, buttonWidth, buttonHeight);
   rect(buttonX + buttonWidth + 10, buttonY, buttonWidth, buttonHeight);

   fill(255, 255, 255);
   text("Clear", buttonX + 14, buttonY + 8);
   text("Write", buttonX + buttonWidth + 24, buttonY + 8);
}

void drawSliderPanel() {
  int textPosition = 60;
  
  drawSlider(sliderX, paletteY, sliderValue1, maxValue);
  text("Red", paletteX + textPosition, paletteY + 5);
  
  drawSlider(sliderX, paletteY + sliderSpacing, sliderValue2, maxValue);
  text("Green", paletteX + textPosition, paletteY + sliderSpacing + 5);
  
  drawSlider(sliderX, paletteY + 2 * sliderSpacing, sliderValue3, maxValue);
  text("Blue", paletteX + textPosition, paletteY + 2 * sliderSpacing + 5);
  
  fill(customColor); 
  rect(paletteX, paletteY + 20, colorBoxWidth, colorBoxHeight);
  
  drawInputFiels();
}

void drawSlider(int x, int y, int sliderValue, int maxValue) {
  fill(150);
  rect(x, y, sliderWidth, sliderHeight);
  
  float knobX = map(sliderValue, 0, maxValue, x, x + sliderWidth);
  fill(255, 0, 0);
  ellipse(knobX, y + sliderHeight / 2, 20, 20);
}

void drawInputFiels() {
  for (int i = 0; i < inputs.length; i++) {
    int y = fieldY + i * (fieldHeight + fieldSpacing);

    stroke(0);
    fill(isFocused[i] ? 255 : 230); 
    rect(fieldX, y, fieldWidth, fieldHeight);

    fill(0);
    textAlign(LEFT, CENTER);
    text(inputs[i], fieldX + 5, y + fieldHeight / 2);
  }
}

void mousePressed() {
  processMatrixBoxClick();
  processInputFieldsClick();
  processColorPaletteClick();
  processButtonClick();
}

void processMatrixBoxClick() {
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

void processInputFieldsClick() {
  for (int i = 0; i < isFocused.length; i++) {
    int y = paletteY + i * (fieldHeight + fieldSpacing);
    if (mouseX > fieldX && mouseX < fieldX + fieldWidth &&
        mouseY > y && mouseY < y + fieldHeight) {
      isFocused[i] = true; 
    } else {
      isFocused[i] = false; 
    }
  }
}

void processColorPaletteClick() {      
  if (mouseY > colorBoxY && mouseY < colorBoxY + 50) {
    if (mouseX > colorBoxX && mouseX < colorBoxX + 50) {
      selectedColor = color(255, 0, 0);  // Red
    } else if (mouseX > colorBoxX + 60 && mouseX < colorBoxX + 110) {
      selectedColor = color(0, 255, 0);  // Green
    } else if (mouseX > colorBoxX + 120 && mouseX < colorBoxX + 170) {
      selectedColor = color(0, 0, 255);  // Blue
    } else if (mouseX > colorBoxX + 180 && mouseX < colorBoxX + 230) {
      selectedColor = color(255, 255, 0);  // Yellow
    } else if (mouseX > colorBoxX + 240 && mouseX < colorBoxX + 290) {
      selectedColor = color(255, 255, 255); // White
    } else if (mouseX > colorBoxX + 300 && mouseX < colorBoxX + 350) {
      selectedColor = color(100, 100, 100); // Grey
    }
  } else if (mouseY > paletteY + 20 && mouseY < paletteY + 20 + colorBoxHeight 
              && mouseX > paletteX && mouseX < paletteX + colorBoxWidth){
    println("custom color");
    selectedColor = customColor;
  }
}

void processButtonClick() {
  if (mouseY > buttonY && mouseY < buttonY + buttonWidth) {
    if(mouseX > buttonX && mouseX < buttonX + buttonWidth) {
      clearMatrix();
    } else if (mouseX > buttonX + buttonWidth + 10 && mouseX<buttonX + 2 *buttonWidth + 10) {
      println("works");
    }
  }
}

void clearMatrix() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      grid[i][j] = color(0); 
    }
  }
}

void writeFiles() {

  
  PrintWriter colorsFile = createWriter("colors.txt");
  PrintWriter keysFile = createWriter("keys.txt");

  // Iterate through the grid to assign keys and write data
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      color c = grid[i][j];

      // If the color is not already mapped, assign it a new key
      if (!colorMap.containsKey(c)) {
        colorMap.put(c, colorKeyCounter);
        colorsFile.println(colorKeyCounter + " -> (" + (int)red(c) + "," + (int)green(c) + "," + (int)blue(c) + ")");
        colorKeyCounter++;
      }

      // Write the key to the keys file
      int key = colorMap.get(c);
      keysFile.print(key + " ");
    }
    keysFile.println(); // Newline after each row
  }

  // Close the files
  colorsFile.close();
  keysFile.close();

  println("Files written: colors.txt and keys.txt");
}

void keyPressed() {
  for (int i = 0; i < inputs.length; i++) {
    if (isFocused[i]) {
      if (!wasCleared[i]) {
        inputs[i] = "";
        wasCleared[i] = true;
      }
      if (key >= '0' && key <= '9') { 
        String fieldValueStr = inputs[i] + key;
        int fieldValue = int(fieldValueStr);
        if (fieldValue <= maxValue) {
          inputs[i] = fieldValueStr;
          updateCustomColor(fieldValue, i);
        }
      } else if (key == BACKSPACE && inputs[i].length() > 0) { 
        inputs[i] = inputs[i].substring(0, inputs[i].length() - 1);
      }
    } else {
      wasCleared[i] = false;
    }
  }
}

void mouseDragged() {
  if (mouseY > sliderY && mouseY < sliderY + 10) { // Slider 1
    sliderValue1 = (int)map(mouseX, sliderX, sliderX + sliderWidth, minValue, maxValue);
    sliderValue1 = constrain(sliderValue1, minValue, maxValue);
    
    updateCustomColor(sliderValue1, 0);
  } else if (mouseY > sliderY + 30 && mouseY < sliderY + 40) { // Slider 2
    sliderValue2 = (int)map(mouseX, sliderX, sliderX + sliderWidth, minValue, maxValue);
    sliderValue2 = constrain(sliderValue2, minValue, maxValue);
    
    updateCustomColor(sliderValue2, 1);
  } else if (mouseY > sliderY + 60 && mouseY < sliderY + 70) { // Slider 3
    sliderValue3 = (int)map(mouseX, sliderX, sliderX + sliderWidth, minValue, maxValue);
    sliderValue3 = constrain(sliderValue3, minValue, maxValue);
    
    updateCustomColor(sliderValue3, 2);
  }
}

void updateCustomColor(int colorValue, int colorPosition) {
  customColorValues[colorPosition] = colorValue;
  inputs[colorPosition] = str(colorValue);
  
  customColor = color(customColorValues[0], customColorValues[1], customColorValues[2]);
  selectedColor = customColor;
}
