import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class HopData {
    private int lineNum_;
    private String ipAddress_;
    private float[] hopTime_ = new float[3];
    private float avgTime_;

    public HopData() {
        lineNum_ = 0;
        ipAddress_ = "";
        avgTime_ = 0;
    }

    public void makeLine(int lineNum) {
        lineNum_ = lineNum;
    }

    public void makeAddress(String address) {
        ipAddress_ = address;
    }

    public void addHopTime(float n1, float n2, float n3) {
        hopTime_[0] = n1;
        hopTime_[1] = n2;
        hopTime_[2] = n3;
        calculateAvg();
    }

    private void calculateAvg() {
        float first = hopTime_[0];
        float second = hopTime_[1];
        float third = hopTime_[2];

        avgTime_ = (first + second + third) / 3;
    }

    public void fileWriteHopData() throws IOException {
        String fullString = lineNum_ + "\n" + "IP Address: " + ipAddress_+ "\n" + "Times: " + hopTime_[0] + " " + hopTime_[1] + " " + hopTime_[2]+ "\n" + "Average Time: " + avgTime_+ "\n";
        String partString = ipAddress_ + "," + avgTime_ + "\n";
        System.out.println(fullString);

        File file = new File("../traceroute_analysis101.txt");
        FileWriter fw = new FileWriter(file, true);

        fw.write(partString);
        fw.close();
    }
}
