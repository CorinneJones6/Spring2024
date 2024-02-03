import java.util.HashMap;

public class DNSCache {

    private static HashMap<DNSQuestion, DNSRecord> cache;

    DNSCache(){
        cache = new HashMap<>();
    }

    public static synchronized void insertRecord(DNSQuestion question, DNSRecord message){
        cache.put(question, message);
    }
    public static synchronized DNSRecord queryRecord(DNSQuestion question){
        return cache.getOrDefault(question, null); //Return record if exists, null otherwise
    }
    public synchronized boolean isCached(DNSQuestion question){
        return cache.containsKey(question);
    }


}
