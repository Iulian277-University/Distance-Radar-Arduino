import processing.serial.*;     // Serial communication with Arduino
import java.awt.event.KeyEvent; // Read data from serial port
import java.io.IOException;     // Exception handling

// Maximum distance in `cm`
final int MAX_DISTANCE = 40;


// `Serial` object to communicate with Arduino
Serial myPort;

//  
String data     = "";
String angle    = "";
String distance = "";

float pixsDistance;
int iAngle, iDistance;
int commaIndex = 0;

void setup() {
  // Screen resolution
  size(1200, 700);

  // Enable anti-aliasing
  smooth();

  // Start the serial communication with Arduino
  // on serial port COM8 at 9600 baud rate  
  myPort = new Serial(this, "COM8", 9600);

  // Read the data from serial port up to the character `.`
  // This means that the data should be in format "angle,distance."
  myPort.bufferUntil('.');
}

void draw() {
  // Draw the background
  fill(98, 245, 31);

  // Simulate motion blur and slow fade of the moving line
  noStroke();
  fill(0, 4); 
  rect(0, 0, width, height - height * 0.065); 
  
  // Draw the radar
  fill(98, 245, 31);

  // Call the functions for drawing the radar
  drawRadar(); 
  drawLine();
  drawObject();
  drawText();
}

void serialEvent(Serial myPort) {
  // Read the data from the Serial Port up to the character `.`
  // and put it into the String variable `data`
  data = myPort.readStringUntil('.');
  data = data.substring(0, data.length() - 1);
  
  // Find the index of the character `,`
  commaIndex = data.indexOf(',');
  
  // Parse the `angle` and `distance` values from the `data` string
  angle    = data.substring(0, commaIndex); 
  distance = data.substring(commaIndex + 1, data.length());
  
  // Converts the `String` variables to `int`
  iAngle    = int(angle);
  iDistance = int(distance);
}

void drawRadar() {
  // Save the current coordinate system transformation state onto the stack
  pushMatrix();

  // Move the coordinate system origin to the center of the screen
  translate(width / 2, height - height * 0.074);

  // Clear the screen  
  noFill();

  // Set line thickness and color (green) for subsequent drawing commands
  strokeWeight(2);
  stroke(98, 245, 31);

  // Draw the arc lines
  arc(0, 0, (width - width * 0.0625), (width - width * 0.0625), PI, TWO_PI);
  arc(0, 0, (width - width * 0.2700), (width - width * 0.2700), PI, TWO_PI);
  arc(0, 0, (width - width * 0.4790), (width - width * 0.4790), PI, TWO_PI);
  arc(0, 0, (width - width * 0.6870), (width - width * 0.6870), PI, TWO_PI);
  
  // Draw the angle lines
  line(-width / 2, 0, width / 2, 0);
  line(0, 0, (-width / 2) * cos(radians(30)),  (-width / 2) * sin(radians(30)));
  line(0, 0, (-width / 2) * cos(radians(60)),  (-width / 2) * sin(radians(60)));
  line(0, 0, (-width / 2) * cos(radians(90)),  (-width / 2) * sin(radians(90)));
  line(0, 0, (-width / 2) * cos(radians(120)), (-width / 2) * sin(radians(120)));
  line(0, 0, (-width / 2) * cos(radians(150)), (-width / 2) * sin(radians(150)));
  line((-width / 2) * cos(radians(30)), 0, width / 2, 0);
  
  // Restore the coordinate system transformation state back from the stack
  popMatrix();
}

void drawObject() {
  // Save the current coordinate system transformation state onto the stack
  pushMatrix();

  // Move the coordinate system origin to the center of the screen
  translate(width / 2, height - height * 0.074);

  // Set the line thickness and color (red) for subsequent drawing commands
  strokeWeight(9);
  stroke(255, 10, 10);

  // Convert the distance from the sensor from `cm` to `pixels`
  pixsDistance = iDistance * ((height - height * 0.1666) * 0.025);

  // Limit the range to `MAX_DISTANCE`
  if (iDistance < MAX_DISTANCE) {
      // Draw the object according to the `angle` and the `distance`
      line(
          pixsDistance * cos(radians(iAngle)),
        - pixsDistance * sin(radians(iAngle)),
          (width - width * 0.505) * cos(radians(iAngle)),
        - (width - width * 0.505) * sin(radians(iAngle))
      );
  }

  // Restore the coordinate system transformation state back from the stack
  popMatrix();
}

void drawLine() {
  // Save the current coordinate system transformation state onto the stack
  pushMatrix();
  
  // Set the line thickness and color (light green) for subsequent drawing commands
  strokeWeight(9);
  stroke(30, 250, 60);

  // Move the coordinate system origin to the center of the screen
  translate(width / 2, height - height * 0.074);

  // Draw the line according to the `angle` and the `distance`
  line(
     0,
     0,
     (height - height * 0.12) * cos(radians(iAngle)),
    -(height - height * 0.12) * sin(radians(iAngle)));

  // Restore the coordinate system transformation state back from the stack
  popMatrix();
}

void drawText() {
  // Save the current coordinate system transformation state onto the stack
  pushMatrix();

  // Draw the background and a rectangle for the text
  fill(0, 0, 0);
  noStroke();
  rect(0, height - height * 0.0648, width, height);
  
  // Set the text color (green) and size  
  fill(98, 245, 31);
  textSize(25);
  
  // Write the 4 landmarks on the screen
  text("10cm", width - width * 0.3854, height - height * 0.0833);
  text("20cm", width - width * 0.2810, height - height * 0.0833);
  text("30cm", width - width * 0.1770, height - height * 0.0833);
  text("40cm", width - width * 0.0729, height - height * 0.0833);
 
  // Set the font size to `40` and write the `angle` and the `distance` values
  textSize(40);
  text("Radar Distance ",         width - width * 0.875, height - height * 0.0277);
  text("Angle: " + iAngle + " °", width - width * 0.480, height - height * 0.0277);
  text("Distance: ",              width - width * 0.260, height - height * 0.0277);

  if (iDistance < MAX_DISTANCE)
      text("" + iDistance + " cm", width - width * 0.125, height - height * 0.0277);

  // Set the font size to `25`
  textSize(25);
  fill(98, 245, 60);

  // Draw the `30` degrees angle text
  translate(
    (width  - width  * 0.4994) + width / 2 * cos(radians(30)),
    (height - height * 0.0907) - width / 2 * sin(radians(30))
  );
  rotate(-radians(-60));
  text("30°", 0, 0);
  resetMatrix();

  // Draw the `60` degrees angle text
  translate(
    (width  - width  * 0.5030) + width / 2 * cos(radians(60)),
    (height - height * 0.0888) - width / 2 * sin(radians(60))
  );
  rotate(-radians(-30));
  text("60°", 0, 0);
  resetMatrix();

  // Draw the `90` degrees angle text
  translate(
    (width  - width  * 0.5070) + width / 2 * cos(radians(90)),
    (height - height * 0.0833) - width / 2 * sin(radians(90))
  );
  rotate(radians(0));
  text("90°", 0, 0);
  resetMatrix();

  // Draw the `120` degrees angle text
  translate(
    (width  - width  * 0.51300) + width / 2 * cos(radians(120)),
    (height - height * 0.07129) - width / 2 * sin(radians(120))
  );
  rotate(radians(-30));
  text("120°", 0, 0);
  resetMatrix();

  // Draw the `150` degrees angle text
  translate(
    (width  - width  * 0.5104) + width / 2 * cos(radians(150)),
    (height - height * 0.0574) - width / 2 * sin(radians(150)));
  rotate(radians(-60));
  text("150°", 0, 0);

  // Restore the coordinate system transformation state back from the stack
  popMatrix(); 
}
