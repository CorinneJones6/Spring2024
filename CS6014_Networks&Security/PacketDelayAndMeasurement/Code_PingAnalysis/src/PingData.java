import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class PingData {
    private float minTime_;
    private float[] pingTimeArr_ = new float[10];
    private int count_;
    private float avgQueueDelay_;

    public PingData() {
        minTime_ = 0;
        count_ = 0;
        avgQueueDelay_ = 0;
    }

    public void insertTime(float f) {

        if (pingTimeArr_.length == count_) {

            // Creating a new array double the size
            float newArr[] = new float[2 * count_];

            // Iterating over new array using for loop
            for (int i = 0; i < count_; i++) {
                newArr[i] = pingTimeArr_[i];
            }

            // Assigning new array to original array
            pingTimeArr_ = newArr;
        }
        //Add the new float
        pingTimeArr_[count_++] = f;
    }

    public void calcMinTime() {
        float min = pingTimeArr_[0];
        for (float time : pingTimeArr_) {
            if (time < min && time != 0) {
                min = time;
            }
        }
        minTime_ = min;
    }

    public void calcAvgQueueDelay() {
        float total = 0;
        for (float time : pingTimeArr_) {
            if (time != 0) {
                total += time - minTime_;
            }
        }
        avgQueueDelay_ = total / count_;
    }

    public void writePingDataToFile() throws IOException {
        String fullString = "Count: " + count_ + "\n" + "Average Queue Delay: " + avgQueueDelay_ + "\n";
        String partString = count_ + "," + avgQueueDelay_ + "\n";
        System.out.println(fullString);

        File file = new File("../Files_PingAnalysis/ping_analysis.txt");
        FileWriter fw = new FileWriter(file, true);

        fw.write(fullString);
        fw.close();
    }

}
