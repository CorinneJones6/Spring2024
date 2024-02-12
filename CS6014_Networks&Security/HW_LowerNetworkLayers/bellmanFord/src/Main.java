import java.util.ArrayList;

public class Main {

    public static void main(String[] args) throws InterruptedException {
        ArrayList<Integer> numMsgs = new ArrayList<Integer>();
        int num = 35;
//        Network.makeSimpleNetwork(); //use this for testing/debugging
        for(int i=0; i<11; i++) {
            Network.makeProbablisticNetwork(num); //use this for the plotting part

            Network.dump();

            Network.startup();
            Network.runBellmanFord();

            System.out.println("done building tables!");
            for (Router r : Network.getRouters()) {
                r.dumpDistanceTable();
            }
            numMsgs.add(Network.getMessageCount());
            System.out.println("total messages: " + Network.getMessageCount());
        }


        System.out.println("Network Size: " + num);

        double count = 0;

        for (Integer numMsg : numMsgs) {
            count +=numMsg;
        }
        double result = count/numMsgs.size();

        System.out.println("Average: " + result);
    }
}