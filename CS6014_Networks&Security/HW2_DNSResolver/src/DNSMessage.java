import java.io.*;
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

    DNSMessage(){

    }
    static DNSMessage decodeMessage(byte[] bytes) throws IOException {
        ByteArrayInputStream inputStream = new ByteArrayInputStream(bytes);

        //todo: implement this function
        return new DNSMessage();
    }

    public String[] readDomainName(InputStream is) {
        //todo: implement this function
        return new String[0];
    }

    public String[] readDomainName(int firstByte){
        //todo: implement this function
        return new String[0];
    }

    static DNSMessage buildResponse(DNSMessage request, DNSRecord[] answers){
        //todo: implement this function
        return new DNSMessage();
    }

    public byte[] toBytes() {
        //todo: implement this function
        return new byte[0];
    }

    static void writeDomainName(ByteArrayOutputStream bo, HashMap<String,Integer> domainLocations, String[] domainPieces){
        //todo: implement this function
    }

    public String joinDomainName(String[] pieces){
        //todo: implement this function
        return "";
    }

    public String toString(){
        //todo: implement this function
        return "";
    }

}
