void drawColorPalette() {
  fill(255, 0, 0);  // Red color
  rect(colorBoxX, colorBoxY, colorBoxWidth, colorBoxHeight);

  fill(0, 255, 0);  // Green color
  rect(colorBoxX + 60, colorBoxY, colorBoxWidth, colorBoxHeight);

  fill(0, 0, 255);  // Blue color
  rect(colorBoxX + 120, colorBoxY, colorBoxWidth, colorBoxHeight);

  fill(255, 255, 0);  // Yellow color
  rect(colorBoxX + 180, colorBoxY, colorBoxWidth, colorBoxHeight);
  
  fill(255, 255, 255);  // White color
  rect(colorBoxX + 240, colorBoxY, colorBoxWidth, colorBoxHeight);
  
  fill(100, 100, 100);  // Grey color
  rect(colorBoxX + 300, colorBoxY, colorBoxWidth, colorBoxHeight);
}

void processColorPaletteClick() {     
  if(mouseButton == RIGHT) return;
  
  if (mouseY > colorBoxY && mouseY < colorBoxY + 50) {
    if (mouseX > colorBoxX && mouseX < colorBoxX + 50) {
      selectedColor = color(255, 0, 0);  // Red
      updateColor(selectedColor);
    } else if (mouseX > colorBoxX + 60 && mouseX < colorBoxX + 110) {
      selectedColor = color(0, 255, 0);  // Green
      updateColor(selectedColor);
    } else if (mouseX > colorBoxX + 120 && mouseX < colorBoxX + 170) {
      selectedColor = color(0, 0, 255);  // Blue
      updateColor(selectedColor);
    } else if (mouseX > colorBoxX + 180 && mouseX < colorBoxX + 230) {
      selectedColor = color(255, 255, 0);  // Yellow
      updateColor(selectedColor);
    } else if (mouseX > colorBoxX + 240 && mouseX < colorBoxX + 290) {
      selectedColor = color(255, 255, 255); // White
      updateColor(selectedColor);
    } else if (mouseX > colorBoxX + 300 && mouseX < colorBoxX + 350) {
      selectedColor = color(100, 100, 100); // Grey
      updateColor(selectedColor);
    }
  } 
  //else if (mouseY > paletteY + 20 && mouseY < paletteY + 20 + colorBoxHeight 
  //            && mouseX > paletteX && mouseX < paletteX + colorBoxWidth){
  //  selectedColor = customColor;
  //}
}
