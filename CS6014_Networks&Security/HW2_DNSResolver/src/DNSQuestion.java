import java.io.*;
import java.util.HashMap;
import java.util.Objects;

/*                                  1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/

public class DNSQuestion {

    private String[] qName_;
    private short qType_, qClass_;

    DNSQuestion() {

    }

    DNSQuestion(String[] qName, short qType, short qClass) {
        qName_ = qName;
        qType_ = qType;
        qClass_ = qClass;
    }

    /**
     * Decodes a DNS question from the provided InputStream and constructs a DNSQuestion object.
     *
     * @param inputStream The InputStream containing the DNS question data.
     * @param dnsMessage  The DNSMessage object to which the question belongs, used for domain name decoding.
     * @return A DNSQuestion object representing the decoded DNS question.
     * @throws IOException If there is an error during the decoding process.
     */
    static DNSQuestion decodeQuestion(InputStream inputStream, DNSMessage dnsMessage) throws IOException {
        DataInputStream dataInputStream = new DataInputStream(inputStream);

        // Decode the domain name for the DNS question
        String[] qName = DNSMessage.readDomainName(inputStream);

        // Read and parse the question type and class
        short qType = dataInputStream.readShort();
        short qClass = dataInputStream.readShort();

        // Create a DNSQuestion object with the decoded information
        return new DNSQuestion(qName, qType, qClass);
    }

    /**
     * Writes the DNSQuestion object as bytes to the provided ByteArrayOutputStream.
     *
     * @param byteArrayOutputStream The ByteArrayOutputStream where the DNSQuestion will be written.
     * @param domainNameLocations   A HashMap mapping domain names to their locations for possible compression.
     * @throws IOException If there is an error during the writing process.
     */
    void writeBytes(ByteArrayOutputStream byteArrayOutputStream, HashMap<String, Integer> domainNameLocations) throws IOException {
        DNSMessage.writeDomainName(byteArrayOutputStream, domainNameLocations, qName_);
        DataOutputStream dataOutputStream = new DataOutputStream(byteArrayOutputStream);

        dataOutputStream.writeShort(qType_);
        dataOutputStream.writeShort(qClass_);
    }

    /**
     * Returns a string representation of the DNSQuestion object.
     *
     * @return A string containing the DNSQuestion's qName, qType, and qClass values.
     */
    @Override
    public String toString() {
        return "DNSQuestion{" +
                "qName_='" + qName_ + '\'' +
                ", qType_=" + qType_ +
                ", qClass_=" + qClass_ +
                '}';
    }

    /**
     * Compares this DNSQuestion object to another object for equality.
     *
     * @param o The object to compare to this DNSQuestion.
     * @return true if the objects are equal, false otherwise.
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DNSQuestion that = (DNSQuestion) o;
        return qType_ == that.qType_ && qClass_ == that.qClass_ && Objects.equals(qName_, that.qName_);
    }

    /**
     * Computes a hash code for the DNSQuestion object.
     *
     * @return The hash code value for this DNSQuestion.
     */
    @Override
    public int hashCode() {
        return Objects.hash(qName_, qType_, qClass_);
    }

}
