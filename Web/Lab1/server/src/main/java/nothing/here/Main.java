package nothing.here;


import java.text.Format;
import java.text.SimpleDateFormat;
import java.util.*;

import com.fastcgi.FCGIInterface;
import nothing.here.commands.AddCommand;
import nothing.here.commands.GetAllCommand;
import nothing.here.commands.ICommand;
import nothing.here.exceptions.EBadResponse;
import nothing.here.exceptions.EHttpResponseBaseError;
import nothing.here.exceptions.EInternalError;


public class Main {
    final static List<RequestData> requestDataList = new ArrayList<>();

    public static void main(String[] args) {

        var fcgiInterface = new FCGIInterface();

        while (fcgiInterface.FCGIaccept() >= 0) {
            try {
                handleRequest();
            } catch (EBadResponse e) {
                String response = ResponseFactory.createBadResponseErrorText(e.getMessage());
                System.out.println(response);
            } catch (EInternalError e) {
                String response = ResponseFactory.createInternalErrorResponseText(e.getMessage());
                System.out.println(response);
            }
            catch(Exception e) {
                String response = ResponseFactory.createBadResponseErrorJson(e.getMessage());
                System.out.println(response);
            }
        }
    }

    private static void handleRequest() throws EHttpResponseBaseError {
        final HashMap<String, ICommand>  commands = new HashMap<>();
        commands.put("/fcgi-bin/server.jar/add", new AddCommand(requestDataList));
        commands.put("/fcgi-bin/server.jar/getAll", new GetAllCommand(requestDataList));

        String uri = FCGIInterface.request.params.getProperty("REQUEST_URI");

        ICommand command = null;
        for (String cmd : commands.keySet()) {
            if (uri.startsWith(cmd)) {
                command = commands.get(cmd);
            }
        }

        if  (command == null) {
            String response = ResponseFactory.createBadResponseErrorText("No such command");
            System.out.println(response);
        }

        String requestMethod = FCGIInterface.request.params.getProperty("REQUEST_METHOD");
        if (!requestMethod.equalsIgnoreCase("get")) {
            String response = ResponseFactory.createBadResponseErrorText("Only GET requests are supported");
            System.out.println(response);
            return;
        }

        System.out.println(command.execute());
    }
}