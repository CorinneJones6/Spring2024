import org.junit.Test;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MainTest {
    @Test
    public void testipPattern() {
        String text = "(10.18.128.3)";
        String ipPattern = "^\\((\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3})\\)$";

        Pattern pattern = Pattern.compile(ipPattern);
        Matcher matcher = pattern.matcher(text);

        boolean letsCheck = matcher.matches();

        if (letsCheck) {
            System.out.println("Pattern Matched");
        } else {
            System.out.println("Pattern not found.");
        }
    }
    @Test
    public void testMSPattern() {
        String text = "123.123 ms";
        String timePattern = ".*\\d{3} ms$";

        Pattern pattern = Pattern.compile(timePattern);
        Matcher matcher = pattern.matcher(text);

        boolean letsCheck = matcher.matches();

        if (letsCheck) {
            System.out.println("Pattern Matched");
        } else {
            System.out.println("Pattern not found.");
        }
    }
}