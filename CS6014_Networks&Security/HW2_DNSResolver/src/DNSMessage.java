import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;

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
    private ArrayList<DNSQuestion> question_;
    private ArrayList<DNSRecord> answer_;
    private ArrayList<DNSRecord> authority_;
    private ArrayList<DNSRecord> additional_;
    private byte[] messageBytes;

    DNSMessage(){

    }
    static DNSMessage decodeMessage(byte[] bytes) throws IOException {
        ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(bytes);
        DataInputStream dataInputStream = new DataInputStream(byteArrayInputStream);

        DNSMessage dnsMessage = new DNSMessage();
        dnsMessage.header_= new DNSHeader();
        dnsMessage.question_ = new ArrayList<>();
        dnsMessage.answer_ = new ArrayList<>();
        dnsMessage.authority_ = new ArrayList<>();
        dnsMessage.additional_ = new ArrayList<>();

        //decode dns header
        dnsMessage.header_ = DNSHeader.decodeHeader(byteArrayInputStream);

        for (int i = 0; i < dnsMessage.header_.getQdCount_(); i++) {
            DNSQuestion question = (DNSQuestion.decodeQuestion(dataInputStream, dnsMessage));
            dnsMessage.question_.add(question);
        }

        for (int i = 0; i < dnsMessage.header_.getAnCount_(); i++) {
            dnsMessage.answer_.add(DNSRecord.decodeRecord(dataInputStream, dnsMessage));
        }

        for (int i = 0; i < dnsMessage.header_.getNsCount_(); i++) {
            dnsMessage.authority_.add(DNSRecord.decodeRecord(dataInputStream, dnsMessage));
        }

        for (int i = 0; i < dnsMessage.header_.getArCount_(); i++) {
            dnsMessage.additional_.add(DNSRecord.decodeRecord(dataInputStream, dnsMessage));
        }

        return dnsMessage;
    }
    public String[] readDomainName(InputStream is) throws IOException {
        DataInputStream inputStream = new DataInputStream(is);
        String qName;
        int len;
        ArrayList<String> arrayList = new ArrayList<>();

        while( (len = inputStream.readByte()) > 0) {
            byte[] record = new byte[len];
            for(int i=0; i<len; i++){
                record[i] = inputStream.readByte();
            }
            qName = new String (record, StandardCharsets.UTF_8);
            arrayList.add(qName);
        }

        String[] returnString= new String[arrayList.size()];

        for(int i=0; i<arrayList.size(); i++){
            returnString[i]=arrayList.get(i);
            System.out.println(returnString[i]);
        }
        return returnString;
    }

    String[] readDomainName(int firstByte) throws IOException {
        ByteArrayInputStream inputStream = new ByteArrayInputStream(messageBytes, firstByte, messageBytes.length-firstByte);
        return readDomainName(inputStream);
    }

    static DNSMessage buildResponse(DNSMessage request, DNSRecord[] answers){
        return null;
    }

    static void writeDomainName(ByteArrayOutputStream bo, HashMap<String,Integer> domainLocations, String[] domainPieces) throws IOException {
        DataOutputStream outputStream = new DataOutputStream(bo);
        String domain = joinDomainName(domainPieces);

        if(domainLocations.containsKey(domain)){
            int location = domainLocations.get(domain);
            outputStream.writeShort(0xC000 | location); //0b1100000000000000
        } else {
            int location = outputStream.size();
            domainLocations.put(domain, location);

            for(String label: domainPieces){
                outputStream.writeByte(label.length());
                outputStream.writeBytes(label);
            }
            outputStream.writeByte(0);
        }
    }

    public static String joinDomainName(String[] pieces){
        return String.join(".", pieces);
    }

    @Override
    public String toString() {
        return "DNSMessage{" +
                "header_=" + header_ +
                ", question_=" + question_ +
                ", answer_=" + answer_ +
                ", authority_=" + authority_ +
                ", additional_=" + additional_ +
                ", messageBytes=" + Arrays.toString(messageBytes) +
                '}';
    }

    //========== Getters ==========//

    public DNSHeader getHeader_() {
        return header_;
    }

    public ArrayList<DNSQuestion> getQuestion_() {
        return question_;
    }

    public ArrayList<DNSRecord> getAnswer_() {
        return answer_;
    }

    public ArrayList<DNSRecord> getAuthority_() {
        return authority_;
    }

    public ArrayList<DNSRecord> getAdditional_() {
        return additional_;
    }
}
