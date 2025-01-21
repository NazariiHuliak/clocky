void drawSliderPanel() {
  int textPosition = 60;
  
  drawSlider(sliderX, paletteY, sliderValues[0], maxValue);
  text("Red", paletteX + textPosition, paletteY + 5);
  
  drawSlider(sliderX, paletteY + sliderSpacing, sliderValues[1], maxValue);
  text("Green", paletteX + textPosition, paletteY + sliderSpacing + 5);
  
  drawSlider(sliderX, paletteY + 2 * sliderSpacing, sliderValues[2], maxValue);
  text("Blue", paletteX + textPosition, paletteY + 2 * sliderSpacing + 5);
  
  fill(selectedColor); 
  rect(paletteX, paletteY + 20, colorBoxWidth, colorBoxHeight);
  
  drawSliderInputFiels();
}

void drawSlider(int x, int y, int sliderValue, int maxValue) {
  fill(150);
  rect(x, y, sliderWidth, sliderHeight);
  
  float knobX = map(sliderValue, 0, maxValue, x, x + sliderWidth);
  fill(255, 0, 0);
  ellipse(knobX, y + sliderHeight / 2, 20, 20);
}

void drawSliderInputFiels() {
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

void processSliderInputFieldsClick() {
  if(mouseButton == RIGHT) return;
  
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


void processSliderInputFieldsKeyPressed() {
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
          updateColor(fieldValue, i);
        }
      } else if (key == BACKSPACE && inputs[i].length() > 0) { 
        inputs[i] = inputs[i].substring(0, inputs[i].length() - 1);
      }
    } else {
      wasCleared[i] = false;
    }
  }
}

void processColorSliderMouseDragged() {
  if (mouseX > sliderX && mouseX < sliderX + sliderWidth) {
    if (mouseY > sliderY && mouseY < sliderY + 10) { // Slider 1
      sliderValues[0] = (int)map(mouseX, sliderX, sliderX + sliderWidth, minValue, maxValue);
      sliderValues[0] = constrain(sliderValues[0], minValue, maxValue);
      
      updateColor(sliderValues[0], 0);
    } else if (mouseY > sliderY + 30 && mouseY < sliderY + 40) { // Slider 2
      sliderValues[1] = (int)map(mouseX, sliderX, sliderX + sliderWidth, minValue, maxValue);
      sliderValues[1] = constrain(sliderValues[1], minValue, maxValue);
      
      updateColor(sliderValues[1], 1);
    } else if (mouseY > sliderY + 60 && mouseY < sliderY + 70) { // Slider 3
      sliderValues[2] = (int)map(mouseX, sliderX, sliderX + sliderWidth, minValue, maxValue);
      sliderValues[2] = constrain(sliderValues[2], minValue, maxValue);
      
      updateColor(sliderValues[2], 2);
    }
  }
}

void updateColor(color newColor) {
  updateColor(int(red(newColor)), 0);
  updateColor(int(green(newColor)), 1);
  updateColor(int(blue(newColor)), 2);
}

void updateColor(int colorValue, int colorPosition) {
  selectedColorValues[colorPosition] = colorValue;
  inputs[colorPosition] = str(colorValue);
  sliderValues[colorPosition] = colorValue;
  
  selectedColor = color(selectedColorValues[0], selectedColorValues[1], selectedColorValues[2]);
}
