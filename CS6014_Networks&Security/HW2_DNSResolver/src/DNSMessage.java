import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

/*  +---------------------
    |        Header       |
    +---------------------+
    |       Question      | the question for the name server
    +---------------------+
    |        Answer       | RRs answering the question
    +---------------------+
    |      Authority      | RRs pointing toward an authority
    +---------------------+
    |      Additional     | RRs holding additional information
    +---------------------+

 */
public class DNSMessage {
    private DNSHeader header_;
    private ArrayList<DNSQuestion> questions_;
    private ArrayList<DNSRecord> answers_;
    private ArrayList<DNSRecord> authorities_;
    private ArrayList<DNSRecord> additionals_;
    static byte[] messageBytes;
    HashMap<String, Integer> domainLocations_;

    DNSMessage() {

    }

    /**
     * Decodes a DNS message from the given byte array and constructs a DNSMessage object.
     *
     * @param bytes The byte array containing the DNS message data.
     * @return A DNSMessage object representing the decoded DNS message.
     * @throws IOException If there is an error during the decoding process.
     */
    static DNSMessage decodeMessage(byte[] bytes) throws IOException {
        ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(bytes);
        DataInputStream dataInputStream = new DataInputStream(byteArrayInputStream);

        // Store the messageBytes for reference, if needed
        messageBytes = bytes;

        // Initialize member variables of DNSMessage to avoid null pointer exceptions
        DNSMessage dnsMessage = new DNSMessage();
        dnsMessage.header_ = new DNSHeader();
        dnsMessage.questions_ = new ArrayList<>();
        dnsMessage.answers_ = new ArrayList<>();
        dnsMessage.authorities_ = new ArrayList<>();
        dnsMessage.additionals_ = new ArrayList<>();

        // Decode the DNSMessage header
        dnsMessage.header_ = DNSHeader.decodeHeader(byteArrayInputStream);

        // Decode DNSQuestion objects based on the header's QDCount
        for (int i = 0; i < dnsMessage.header_.getQdCount_(); i++) {
            DNSQuestion question = (DNSQuestion.decodeQuestion(dataInputStream, dnsMessage));
            dnsMessage.questions_.add(question);
        }

        // Decode DNSRecord objects from the answer section based on ANCount
        for (int i = 0; i < dnsMessage.header_.getAnCount_(); i++) {
            dnsMessage.answers_.add(DNSRecord.decodeRecord(dataInputStream, dnsMessage));
        }

        // Decode DNSRecord objects from the authority section based on NSCount
        for (int i = 0; i < dnsMessage.header_.getNsCount_(); i++) {
            dnsMessage.authorities_.add(DNSRecord.decodeRecord(dataInputStream, dnsMessage));
        }

        // Decode DNSRecord objects from the additional section based on ARCount
        for (int i = 0; i < dnsMessage.header_.getArCount_(); i++) {
            dnsMessage.additionals_.add(DNSRecord.decodeRecord(dataInputStream, dnsMessage));
        }

        return dnsMessage;
    }

    /**
     * Reads and decodes a domain name from the given InputStream.
     *
     * @param inputStream The InputStream containing the domain name data.
     * @return An array of labels representing the decoded domain name.
     * @throws IOException If there is an error during the reading or decoding process.
     */
    public static String[] readDomainName(InputStream inputStream) throws IOException {
        List<String> domainLabels = new ArrayList<>();
        DataInputStream dataInputStream = new DataInputStream(inputStream);
        // Read the length of the first label
        byte labelLength = dataInputStream.readByte();

        // If the length is zero, return an empty array (root domain)
        if (labelLength == 0) {
            return new String[0];
        }

        // Loop to read and decode labels until a zero-length label is encountered
        while (labelLength != 0) {
            byte[] buffer;

            // Read the label bytes based on the length
            buffer = dataInputStream.readNBytes(labelLength);

            // Convert the label bytes to a String using UTF-8 encoding
            String label = new String(buffer, StandardCharsets.UTF_8);

            // Add the decoded label to the list
            domainLabels.add(label);

            // Read the length of the next label
            labelLength = dataInputStream.readByte();
        }
        // Convert the list of labels to an array and return it
        return domainLabels.toArray(new String[0]);
    }

    /**
     * Reads and decodes a domain name from a byte array starting at the specified index.
     *
     * @param firstByte The index within the byte array from which to start reading the domain name.
     * @return An array of labels representing the decoded domain name.
     * @throws IOException If there is an error during the reading or decoding process.
     */
    public static String[] readDomainName(int firstByte) throws IOException {
        ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(messageBytes, firstByte, messageBytes.length - firstByte);
        return readDomainName(byteArrayInputStream);
    }

    /**
     * Builds a DNS response message based on the provided DNS request message and answer records.
     *
     * @param request       The DNSMessage object representing the original DNS request.
     * @param answerRecords An ArrayList of DNSRecord objects representing the answer records for the response.
     * @return A DNSMessage object representing the DNS response message.
     */
    static DNSMessage buildResponse(DNSMessage request, ArrayList<DNSRecord> answerRecords) {
        DNSMessage response = new DNSMessage();
        response.header_ = DNSHeader.buildHeaderForResponse(request);
        response.questions_ = request.questions_;
        response.answers_ = answerRecords;
        response.authorities_ = new ArrayList<>();
        response.additionals_ = new ArrayList<>();
        return response;
    }

    /**
     * Converts the DNSMessage object into a byte array representation suitable for network transmission.
     *
     * @return A byte array containing the serialized DNSMessage data.
     * @throws IOException If there is an error during the serialization process.
     */
    public byte[] toBytes() throws IOException {
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();

        // Serialize the DNSMessage header and initialize the domainLocations map
        header_.writeBytes(byteArrayOutputStream);
        domainLocations_ = new HashMap<>();

        // Serialize DNSQuestion objects and update domainLocations map
        for (DNSQuestion question : questions_) {
            question.writeBytes(byteArrayOutputStream, domainLocations_);
        }

        // Serialize DNSRecord objects from the answer section
        for (DNSRecord answer : answers_) {
            answer.writeBytes(byteArrayOutputStream, domainLocations_);
        }

        // Serialize DNSRecord objects from the authority section
        for (DNSRecord authority : authorities_) {
            authority.writeBytes(byteArrayOutputStream, domainLocations_);
        }

        // Serialize DNSRecord objects from the additional section
        for (DNSRecord additional : additionals_) {
            additional.writeBytes(byteArrayOutputStream, domainLocations_);
        }

        // Convert the ByteArrayOutputStream to a byte array and return it
        return byteArrayOutputStream.toByteArray();
    }

    /**
     * Writes a domain name to the provided ByteArrayOutputStream using compression if possible.
     *
     * @param byteArrayOutputStream The ByteArrayOutputStream where the domain name will be written.
     * @param domainLocations       A HashMap mapping domain names to their locations for compression.
     * @param domainPieces          An array of labels representing the domain name to be written.
     * @throws IOException If there is an error during the writing process.
     */
    static void writeDomainName(ByteArrayOutputStream byteArrayOutputStream, HashMap<String, Integer> domainLocations, String[] domainPieces) throws IOException {
        DataOutputStream outputStream = new DataOutputStream(byteArrayOutputStream);
        String domainName = joinDomainName(domainPieces);
        if (domainName.length() == 0) {
            outputStream.writeByte(0);
        } else {
            // Write each label of the domain name along with their lengths
            for (String label : domainPieces) {
                byte[] labelBytes = label.getBytes(StandardCharsets.UTF_8);
                outputStream.writeByte(labelBytes.length);
                outputStream.write(labelBytes);
            }
        }
        // Terminate the domain name with a null byte
        outputStream.writeByte(0);
    }

    /**
     * Joins an array of domain name pieces into a single dot-separated domain name string.
     *
     * @param pieces An array of labels representing the domain name pieces to be joined.
     * @return A single domain name string with labels separated by dots.
     */
    public static String joinDomainName(String[] pieces) {
        return String.join(".", pieces);
    }

    /**
     * Returns a string representation of the DNSMessage object.
     *
     * @return A string containing the values of the DNSMessage's fields.
     */
    @Override
    public String toString() {
        return "DNSMessage{" +
                "header_=" + header_ +
                ", question_=" + questions_ +
                ", answer_=" + answers_ +
                ", authority_=" + authorities_ +
                ", additional_=" + additionals_ +
                ", messageBytes=" + Arrays.toString(messageBytes) +
                '}';
    }

    /**
     * Getter methods for retrieving DNSMessage member variables.
     */
    public DNSHeader getHeader_() {
        return header_;
    }

    public ArrayList<DNSQuestion> getQuestions_() {
        return questions_;
    }

    public ArrayList<DNSRecord> getAnswers_() {
        return answers_;
    }

    public ArrayList<DNSRecord> getAuthorities_() {
        return authorities_;
    }

    public ArrayList<DNSRecord> getAdditionals_() {
        return additionals_;
    }
}
