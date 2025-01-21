float boxesSliderX = 10; 
float boxesSliderY = 300; 
float boxesSliderWidth = 570; 
int rectWidth = 50; 
int rectHeight = 30; 
float offsetX = 0; 
float thumbX = boxesSliderX; 
float thumbWidth = 50; 
int totalWidth = 0; 
boolean dragging = false; 


void drawScrollableColorPalette() {
  if (colorMap.isEmpty()) return;

  int index = 0;
  for (var entry : colorMap.entrySet()) {
    float rectX = boxesSliderX + index * rectWidth - offsetX;
    if (rectX + rectWidth > boxesSliderX && rectX < boxesSliderX + boxesSliderWidth) {
      color currentColor = parseColor(entry.getKey());
      fill(currentColor);
      rect(rectX, boxesSliderY - 20, rectWidth, rectHeight);
    }
    index++;
  }
  
  float totalContentWidth = colorMap.size() * rectWidth;
  if (totalContentWidth > boxesSliderWidth) {
    drawScrollableColorPaletteSlider();
  }
}

void drawScrollableColorPaletteSlider() {
  fill(150); 
  rect(boxesSliderX, boxesSliderY + 20, boxesSliderWidth, 10);

  fill(100); 
  rect(thumbX, boxesSliderY + 15, thumbWidth, 20);
}

void processScrollablePaletteClick() {
  if (mouseX > thumbX && mouseX < thumbX + thumbWidth &&
    mouseY > boxesSliderY + 15 &&
    mouseY < boxesSliderY + 35) {
    dragging = true;
  }
   
  int index = 0;
  for (var entry : colorMap.entrySet()) {
    float rectX = boxesSliderX + index * rectWidth - offsetX;
    if (mouseX > rectX && mouseX < rectX + rectWidth && 
        mouseY > boxesSliderY - 20 && mouseY < boxesSliderY + 10) {
        updateColor(parseColor(entry.getKey()));
        break;
    }
    index++;
  }
}

void processScrollablePaletteMouseDragged() {
  if (dragging) {
    
    float totalContentWidth = colorMap.size() * rectWidth;
    if (totalContentWidth > boxesSliderWidth) {
      thumbX += mouseX - pmouseX;
      
      thumbWidth = max(50, (boxesSliderWidth * boxesSliderWidth) / totalContentWidth);
      
      float maxThumbX = boxesSliderX + boxesSliderWidth - thumbWidth;
      thumbX = constrain(thumbX, boxesSliderX, maxThumbX);
      
      float maxOffset = totalContentWidth - boxesSliderWidth;
      offsetX = map(thumbX,
                   boxesSliderX,
                   maxThumbX,
                   0,
                   maxOffset);
    } else {
      thumbX = boxesSliderX;
      offsetX = 0;
    }
  }
}

color parseColor(String colorStr) {
  try {
    var colorComponents = colorStr.split(",");
    return color(Integer.parseInt(colorComponents[0]), Integer.parseInt(colorComponents[1]), Integer.parseInt(colorComponents[2]));
  } catch (Exception e) {
      System.out.println("An unexpected error occurred parsing the color");
      return color(0,0,0);
  }
}
