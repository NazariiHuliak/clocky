import javax.swing.*;
import java.awt.*;

int secondButtonY = buttonY + buttonHeight + 20;
String selectedFile;

void drawButtonsPanel() {
   fill(100, 100, 100);  // grey color
   rect(buttonX, buttonY, buttonWidth, buttonHeight);
   rect(buttonX + buttonWidth + 10, buttonY, buttonWidth, buttonHeight);
    
   rect(buttonX, secondButtonY, buttonWidth, buttonHeight);
   rect(buttonX + buttonWidth + 10, secondButtonY, buttonWidth, buttonHeight); 
    
   fill(255, 255, 255);
   text("Clear", buttonX + 25, buttonY + 8);
   text("Download", buttonX + buttonWidth + 24, buttonY + 8);
   
   text("Import colors", buttonX + 5, secondButtonY + 8);
   text("Import icon", buttonX + buttonWidth + 20, secondButtonY + 8);
   
   drawFilePathInputField();
}

void drawFilePathInputField() {
  fill(0, 0, 0);
  text("File path:", fileInputX, fileInputY - 10);
  
  stroke(0);
  fill(isFileInputFocused ? 255 : 230); 
  rect(fileInputX, fileInputY, fieldPathInputWidth, fieldHeight);
  
  fill(0);
  textAlign(LEFT, CENTER);
  text(filePathInput, fileInputX + 5, fileInputY + fieldHeight / 2);
}

void processButtonsClick() {
  if(mouseButton == RIGHT) return;
  
  if (mouseY > buttonY && mouseY < buttonY + buttonHeight) {
    if(mouseX > buttonX && mouseX < buttonX + buttonWidth) {
      clearMatrix();
    } else if (mouseX > buttonX + buttonWidth + 10 && mouseX < buttonX + 2 * buttonWidth + 10) {
      writeFiles();
    }
  }
  
  if (mouseY > secondButtonY && mouseY < secondButtonY + buttonHeight) {
    if(mouseX > buttonX && mouseX < buttonX + buttonWidth) {
      String colorsData = showTextInputDialog("Enter existing colors: ");
      if(colorsData != "" && colorsData != null) {
        importColors(colorsData);
      } 
    } else if (mouseX > buttonX + buttonWidth + 10 && mouseX < buttonX + 2 * buttonWidth + 10) {
      String iconData = showTextInputDialog("Enter icon matrix: ");
      if(iconData != "" && iconData != null) {
        importIcon(iconData);
      } 
    }
  }
}

void processFilePathInputFieldClick() {
  if(mouseButton == RIGHT) return;
  
  if (mouseY > fileInputY && mouseY < fileInputY + fieldHeight &&
      mouseX > fileInputX && mouseX < fileInputX + fieldPathInputWidth) {
    isFileInputFocused = true;    
  } else {
    isFileInputFocused = false;
  }
}

void processFilePathInputFieldKeyPressed() {
  if (isFileInputFocused) {
    if (key == BACKSPACE && filePathInput.length() > 0) { 
      filePathInput = filePathInput.substring(0, filePathInput.length() - 1);
    } else if (key != BACKSPACE && isValidFilePathCharacter(key)){
      filePathInput += key;
    }
  }
}

void clearMatrix() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      grid[i][j] = color(0); 
    }
  }
  
  colorMap = new HashMap<String, Integer>();
}

String showTextInputDialog(String title) {
  JTextArea textArea = new JTextArea(20, 50); 
  textArea.setLineWrap(true);
  textArea.setWrapStyleWord(true);

  JScrollPane scrollPane = new JScrollPane(textArea);
  scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
  scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

  int result = JOptionPane.showConfirmDialog(
    null, 
    scrollPane, 
    title, 
    JOptionPane.OK_CANCEL_OPTION, 
    JOptionPane.PLAIN_MESSAGE
  );

  if (result == JOptionPane.OK_OPTION) {
    return textArea.getText();
  } else {
    return null;
  }
}

boolean isValidFilePathCharacter(char key) {
    String validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 /\\._-:";
    return validChars.indexOf(key) != -1;
}
