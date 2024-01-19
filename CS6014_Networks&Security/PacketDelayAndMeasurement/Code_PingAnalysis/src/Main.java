import java.io.File;
import java.io.IOException;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        PingData pingData = new PingData();
        String filePath = "../Files_PingAnalysis/output_ping.txt";
        String timePattern = "time=\\d+(\\.\\d+)? ms";

        try {
            //File to read in
            File file = new File(filePath);

            // Create a FileReader to read the file
            Scanner sc = new Scanner(file);

            while (sc.hasNext()) {

                sc.nextLine();

                String timeStr = sc.findInLine(timePattern);

                if (timeStr != null) {
                    String t = timeStr.replaceAll("time=", "");
                    float time = Float.parseFloat(t.replaceAll(" ms", "")); //converts the times to floats
                    pingData.insertTime(time);
                }
            }
            
            pingData.calcMinTime();
            pingData.calcAvgQueueDelay();
            pingData.writePingDataToFile();
            sc.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}