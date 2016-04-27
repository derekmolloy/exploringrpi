#include<gtk/gtk.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<stdlib.h>
#include<wiringPi.h>
using namespace std;

#define USING_DHT11      false   // The DHT11 uses only 8 bits
#define DHT_GPIO         22      // Using GPIO 22 for this example
#define LH_THRESHOLD     26      // Low=~14, High=~38 - pick avg.
float temperature;
float humidity;

int readDHTSensor(){
   wiringPiSetupGpio();
   piHiPri(99);
   int humid=0, temp=0;
   unsigned char data[5] = {0,0,0,0,0};
   pinMode(DHT_GPIO, OUTPUT);                 // gpio starts as output
   digitalWrite(DHT_GPIO, LOW);               // pull the line low
   usleep(18000);                             // wait for 18ms
   digitalWrite(DHT_GPIO, HIGH);              // set the line high
   pinMode(DHT_GPIO, INPUT);                  // now gpio is an input

   // need to ignore the first and second high after going low
   do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==HIGH);
   do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==LOW);
   do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==HIGH);
   // Remember the highs, ignore the lows -- a good philosophy!
   for(int d=0; d<5; d++) {       // for each data byte
      // read 8 bits
      for(int i=0; i<8; i++) {    // for each bit of data
         do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==LOW);
         int width = 0;           // measure width of each high
         do {
            width++;
            delayMicroseconds(1);
            if(width>1000) break; // missed a pulse -- data invalid!
         } while(digitalRead(DHT_GPIO)==HIGH);    // time it!
         // shift in the data, msb first if width > the threshold
         data[d] = data[d] | ((width > LH_THRESHOLD) << (7-i));
      }
   }
   if (USING_DHT11){
      humid = data[0] * 10;            // one byte - no fractional part
      temp = data[2] * 10;             // multiplying to keep code concise
   }
   else {                              // for DHT22 (AM2302/AM2301)
      humid = (data[0]<<8 | data[1]);  // shift MSBs 8 bits left and OR LSBs
      temp = (data[2]<<8 | data[3]);   // same again for temperature
   }
   unsigned char chk = 0;   // the checksum will overflow automatically
   for(int i=0; i<4; i++){ chk+= data[i]; }
   if(chk==data[4]){
      cout << "The checksum is good" << endl;
      temperature = ((float)temp)/10;
      humidity = ((float)humid)/10;
   }
   else {
      cout << "Checksum bad - data error" << endl;
      return -1;
   }
   return 0;
}

// The callback function that is associated with the button
//  it passes a pointer to the label, so that it can be changed
//  when the button is pressed
static void getReadings(GtkWidget *widget, gpointer read_label){
   // cast the generic gpointer into a GtkWidget label
   GtkWidget *reading_label = (GtkWidget *) read_label;
   while (readDHTSensor()==-1){
      usleep(2000000);             // sleep for 2 seconds
   };
   stringstream ss;
   ss << "Reading: Temperature="  << temperature << "℃  Humidity=" << humidity << "%";
   // set the text in the label
   gtk_label_set_text( GTK_LABEL(reading_label), ss.str().c_str());
   ss << endl;  // add a \n to the string for the standard output
   g_print(ss.str().c_str());  //output to the terminal (std out)
}

int main(int argc, char *argv[]){

   GtkWidget *window, *reading_label, *button, *button_label;
   gtk_init(&argc, &argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW (window), "Exploring RPi");

   // Fix the size of the window so that it cannot be resized
   gtk_widget_set_size_request(window, 220, 50);
   gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
   // Place a border of 5 pixels around the inner window edge
   gtk_container_set_border_width (GTK_CONTAINER (window), 5);

   // Quit application if X button is pressed
   g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

   // Set up the window to contain two vertically stacked widgets using a vbox
   GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);  //spacing of 5
   gtk_container_add (GTK_CONTAINER (window), box); //add box to window
   gtk_widget_show (box); // set the vbox visible

   // This is the label in which to display the temperature
   reading_label = gtk_label_new ("Reading is Undefined");
   gtk_widget_show(reading_label);  // make it visible
   gtk_label_set_justify( GTK_LABEL(reading_label), GTK_JUSTIFY_LEFT);
   // Add the label to the vbox
   gtk_box_pack_start (GTK_BOX (box), reading_label, FALSE, FALSE, 0);

   // Create a button and connect it to the getTemperature() callback function
   button = gtk_button_new();
   button_label = gtk_label_new ("Get Reading"); //label for text on button
   gtk_widget_show(button_label);  // show label
   gtk_widget_show(button);        // show button
   gtk_container_add(GTK_CONTAINER (button), button_label);  // add label to button
   // Connect the callback function getReadings() to the button press
   g_signal_connect(button, "clicked", G_CALLBACK (getReadings), (gpointer) reading_label);
   // Add the button to the box
   gtk_box_pack_start (GTK_BOX (box), button, FALSE, FALSE, 0);

   gtk_widget_show(window);
   gtk_main();
   return 0;
}
