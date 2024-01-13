import java.io.File;
import java.io.IOException;
import java.util.Scanner;
import java.util.regex.Pattern;

public class Main {
    public static void main(String[] args) {
        HopData hopData = new HopData();
        String filePath = "../traceroute_output1.txt";
        String timePattern = "\\d+(\\.\\d+)? ms";
        String zeroTo255 = "(\\d{1,2}|(0|1)\\" + "d{2}|2[0-4]\\d|25[0-5])";
        // Regex for a digit from 0 to 255 and followed by a dot, repeat 4 times.
        // This is the regex to validate an IP address.
        // The format for the IP Strings was found from https://www.geeksforgeeks.org/how-to-validate-an-ip-address-using-regular-expressions-in-java/
        Pattern IPPattern = Pattern.compile("\\(" +zeroTo255 + "\\."
                + zeroTo255 + "\\."
                + zeroTo255 + "\\."
                + zeroTo255 + "\\)");

        try {
            //File to read in
            File file = new File(filePath);

            // Create a FileReader to read the file
            Scanner sc = new Scanner(file);

            while (sc.hasNextInt()) { //next int since each line starts with an int

                int lineNum = sc.nextInt(); //moves to the correct starting position

                String ipStr = sc.findWithinHorizon(IPPattern, 100); //finds the IP Address

                String timeStr1 = sc.findWithinHorizon(timePattern, 100); //finds the next three times
                String timeStr2 = sc.findWithinHorizon(timePattern, 100);
                String timeStr3 = sc.findWithinHorizon(timePattern, 100);

                float time1= Float.parseFloat(timeStr1.replaceAll(" ms", "")); //converts the times to floats
                float time2 = Float.parseFloat(timeStr2.replaceAll(" ms", "")); //while removing the 'ms'
                float time3 = Float.parseFloat(timeStr3.replaceAll(" ms", ""));


                hopData.makeLine(lineNum); //update the hopData object member variables
                hopData.makeAddress(ipStr); //update the hopData object member variables
                hopData.addHopTime(time1, time2, time3); //update the hopData object member variables
                hopData.fileWriteHopData(); //print the HopData to a file
            }

            sc.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}