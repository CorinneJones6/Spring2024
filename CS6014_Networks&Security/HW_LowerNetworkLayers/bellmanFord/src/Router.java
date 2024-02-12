import java.util.HashMap;
import java.util.Set;

public class Router {

    private HashMap<Router, Integer> distances;
    private String name;

    public Router(String name) {
        this.distances = new HashMap<>();
        this.name = name;
    }

    public void onInit() throws InterruptedException {

        this.distances.put(this, 0); // initialize self with an integer of 0

        for(Neighbor neighbor : Network.getNeighbors(this)){
            this.distances.put(neighbor.router, neighbor.cost);
        }

        for(Neighbor neighbor: Network.getNeighbors(this)){
            Network.sendDistanceMessage(new Message(this, neighbor.router, new HashMap<>(this.distances)));
        }
    }

    public void onDistanceMessage(Message message) throws InterruptedException {
        boolean updated = false;
        HashMap<Router, Integer> senderDistanceTable = message.distances;
        Router sender = message.sender;
        Integer distanceToSender = this.distances.get(sender);

        if(distanceToSender == null){
            //handle if there is no distance to sender
            distanceToSender = Integer.MAX_VALUE;
            this.distances.put(sender, distanceToSender);
        }

        for (HashMap.Entry<Router, Integer> entry : senderDistanceTable.entrySet()) {

            Router currentRouter = entry.getKey();
            Integer currentDistance = entry.getValue();
            Integer currentKnownDistance = this.distances.getOrDefault(currentRouter, Integer.MAX_VALUE);

            int calculatedDistance = currentDistance + distanceToSender;

            if(calculatedDistance < currentKnownDistance){
                this.distances.put(currentRouter, calculatedDistance);
                updated = true;
            }
        }

        if(updated) {
            //send updated message to all others
            for (Neighbor neighbor : Network.getNeighbors(this)) {
                Network.sendDistanceMessage(new Message(this, neighbor.router, new HashMap<>(this.distances)));
            }
        }
    }


    public void dumpDistanceTable() {
        System.out.println("router: " + this);
        for (Router r : distances.keySet()) {
            System.out.println("\t" + r + "\t" + distances.get(r));
        }
    }

    @Override
    public String toString() {
        return "Router: " + name;
    }
}