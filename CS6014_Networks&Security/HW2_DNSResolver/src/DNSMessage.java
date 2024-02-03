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
    private ArrayList<DNSQuestion> question_;
    private ArrayList<DNSRecord> answer_;
    private ArrayList<DNSRecord> authority_;
    private ArrayList<DNSRecord> additional_;

    static byte[] messageBytes;

    HashMap<String, Integer> domainLocations_;

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
    public static String[] readDomainName(InputStream is) throws IOException {
        List<String> labels = new ArrayList<>();
        DataInputStream stream = new DataInputStream(is);
        byte length = stream.readByte();
        if(length==0){
            return new String[0];
        }
        while(length!=0){
            byte[] buffer;
            buffer = stream.readNBytes(length);
            String str = new String(buffer, StandardCharsets.UTF_8);
            labels.add(str);
            length = stream.readByte();
        }
        return labels.toArray(new String[0]);
    }

    public static String[] readDomainName(int firstByte) throws IOException {
        ByteArrayInputStream inputStream = new ByteArrayInputStream(messageBytes, firstByte, messageBytes.length-firstByte);
        return readDomainName(inputStream);
    }

    static DNSMessage buildResponse(DNSMessage request, ArrayList<DNSRecord> answer){
      DNSMessage response = new DNSMessage();
      response.header_ = DNSHeader.buildHeaderForResponse(request);
      response.question_ = request.question_;
      response.answer_ = answer;
      response.authority_ = new ArrayList<>();
      response.additional_ = new ArrayList<>();
      return response;
    }

    public byte[] toBytes() throws IOException {
        //Convert the DNS message into a byte array for transmission
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        header_.writeBytes(byteArrayOutputStream);
        //Hashmap for domainLocations
        domainLocations_ = new HashMap<>();

        for (DNSQuestion question: question_){
            question.writeBytes(byteArrayOutputStream, domainLocations_);
        }
        for(DNSRecord answer: answer_){
            answer.writeBytes(byteArrayOutputStream, domainLocations_);
        }
        for(DNSRecord authority: authority_){
            authority.writeBytes(byteArrayOutputStream, domainLocations_);
        }
        for(DNSRecord additional: additional_){
            additional.writeBytes(byteArrayOutputStream, domainLocations_);
        }
        return byteArrayOutputStream.toByteArray();
    }

    static void writeDomainName(ByteArrayOutputStream bo, HashMap<String, Integer> domainLocations, String[] domainPieces) throws IOException {
        DataOutputStream outputStream = new DataOutputStream(bo);
        String domainName = joinDomainName(domainPieces);

        if (domainLocations.containsKey(domainName)) {
            int location = domainLocations.get(domainName);
            outputStream.writeShort(0xC000 | location);

        } else {
            int location = outputStream.size();
            domainLocations.put(domainName, location);

            for(String label: domainPieces){
                byte[] labelBytes = label.getBytes(StandardCharsets.UTF_8);
                outputStream.writeByte(labelBytes.length);
                outputStream.write(labelBytes);
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
