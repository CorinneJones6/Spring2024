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
        if (!cache.containsKey(question)) {
            cache.put(question, message);
        }
    }

    /**
     * Queries the cache for a DNS record associated with a given DNS question.
     *
     * @param question The DNSQuestion for which to retrieve the associated DNSRecord.
     * @return The DNSRecord associated with the question, or null if not found in the cache
     * or if the cached record has expired.
     */
    public static synchronized DNSRecord queryRecord(DNSQuestion question) {
        if (!cache.containsKey(question)) {
            // Return null for records not found in the cache
            return null;
        }

        DNSRecord record = cache.get(question);

        if (record.isExpired()) {
            cache.remove(question);
            // Return null for expired records
            return null;
        }

        // Return the non-expired record
        return record;
    }

    /**
     * Checks if a DNS question is cached in the DNSCache.
     *
     * @param question The DNSQuestion to check for in the cache.
     * @return true if the DNS question is cached, false otherwise.
     */
    public static synchronized boolean isCached(DNSQuestion question) {
        return cache.containsKey(question);
    }

}
