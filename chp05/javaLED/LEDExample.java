/** Simple LED Java Example. Written by Derek Molloy (derekmolloy.ie) for the book
*   Exploring Raspberry Pi
*/

package exploringRPi;
import java.io.*;

public class LEDExample {

  private static String GPIO4_PATH = "/sys/class/gpio/gpio4/";
  private static String GPIO_SYSFS = "/sys/class/gpio/";

  private static void writeSysfs(String filename, String value, String path){
     try{
        BufferedWriter bw = new BufferedWriter(new FileWriter (path+filename));
        bw.write(value);
        bw.close();
     }
     catch(IOException e){
        System.err.println("Failed to access the RPi Sysfs file: " + filename);
     }
  }

  public static void main(String[] args) {
     System.out.println("Starting the LED Java Application");
     if(args.length!=1) {
        System.out.println("There is an incorrect number of arguments.");
        System.out.println("  Correct usage is: LEDExample command");
        System.out.println("where command is one of setup, on, off, status, or close");
        System.exit(2);
     }
     if (args[0].equalsIgnoreCase("On") || args[0].equalsIgnoreCase("Off")){
        System.out.println("Turning the LED " + args[0]);
        writeSysfs("value", args[0].equalsIgnoreCase("On")? "1":"0", GPIO4_PATH);
     }
     else if (args[0].equalsIgnoreCase("setup")){
        System.out.println("Setting up the LED");
        writeSysfs("export", "4", GPIO_SYSFS);
        try{
           Thread.sleep(100);               //sleep to ensure that gpio is exported
        } catch(InterruptedException e){}
        writeSysfs("direction", "out", GPIO4_PATH);
     }
     else if (args[0].equalsIgnoreCase("close")){
        System.out.println("Closing down the LED");
        writeSysfs("unexport", "4", GPIO_SYSFS);
     }
     else if (args[0].equalsIgnoreCase("status")){
        try{
           BufferedReader br = new BufferedReader(new FileReader(GPIO4_PATH+"value"));
           String line;
           while ((line = br.readLine()) != null) {
              System.out.println(line);
           }
           br.close();
        }
        catch(IOException e){
           System.err.println("Failed to access the sysfs entry: /value");
        }
     }
     else {
        System.out.println("Invalid command");
     }
  }
}
