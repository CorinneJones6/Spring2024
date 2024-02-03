import java.util.HashMap;

public class DNSCache {

    private static HashMap<DNSQuestion, DNSRecord> cache;

    DNSCache() {
        cache = new HashMap<>();
    }

    /**
     * Inserts a DNS question-answer pair into the cache.
     *
     * @param question The DNSQuestion to use as the key for caching.
     * @param message  The DNSRecord to be associated with the question in the cache.
     */
    public static synchronized void insertRecord(DNSQuestion question, DNSRecord message) {
        cache.put(question, message);
    }

    /**
     * Queries the cache for a DNS record associated with a given DNS question.
     *
     * @param question The DNSQuestion for which to retrieve the associated DNSRecord.
     * @return The DNSRecord associated with the question, or null if not found in the cache.
     */
    public static synchronized DNSRecord queryRecord(DNSQuestion question) {
        return cache.getOrDefault(question, null); //Return record if exists, null otherwise
    }

    /**
     * Checks if a DNS question is cached in the DNSCache.
     *
     * @param question The DNSQuestion to check for in the cache.
     * @return true if the DNS question is cached, false otherwise.
     */
    public synchronized boolean isCached(DNSQuestion question) {
        return cache.containsKey(question);
    }
}
