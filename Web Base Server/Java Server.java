import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/api")
public class ESP32Controller {

    @GetMapping("/scan")
    public String scanNetworks() {
        // Handle network scanning request
        return "Scanning networks...";
    }

    @GetMapping("/sniff")
    public String startSniffing() {
        // Handle sniffing request
        return "Sniffing traffic...";
    }

    @PostMapping("/logs")
    public String downloadLogs() {
        // Handle log download
        return "Logs downloaded!";
    }

    @PostMapping("/settings")
    public String updateSettings(@RequestBody String settings) {
        // Handle settings update
        return "Settings updated!";
    }
}
