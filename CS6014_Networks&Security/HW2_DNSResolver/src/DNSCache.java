import java.util.HashMap;

public class DNSCache {

    private HashMap<DNSQuestion, DNSRecord> cache;

    DNSCache(){
        this.cache = new HashMap<>();
    }

    //Handle inserting the record
    public synchronized void insertRecord(DNSQuestion question, DNSRecord answer){
//        DNSRecord.creationDate = new Date();
        cache.put(question, answer);
    }
    public synchronized DNSRecord queryRecord(DNSQuestion question){
//        checkAndCleanExpiredRecords();
        //Return the record if it exists, null otherwise
        return cache.getOrDefault(question, null);
    }


}
