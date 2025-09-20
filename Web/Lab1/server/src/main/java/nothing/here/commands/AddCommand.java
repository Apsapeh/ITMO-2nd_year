package nothing.here.commands;

import com.fastcgi.FCGIInterface;
import nothing.here.HitChecker;
import nothing.here.QueryParser;
import nothing.here.RequestData;
import nothing.here.ResponseFactory;
import nothing.here.exceptions.EHttpResponseBaseError;

import java.util.List;


public class AddCommand implements ICommand {
    List<RequestData> requests;

    public AddCommand(List<RequestData> requests) {
        this.requests = requests;
    }

    @Override
    public String execute() throws EHttpResponseBaseError {
        String queryString = FCGIInterface.request.params.getProperty("QUERY_STRING");
        List<RequestData> parsedQueries = QueryParser.parse(queryString);

        requests.addAll(parsedQueries);

        return ResponseFactory.createOkResponseJson(RequestData.listToJson(parsedQueries));
    }
}
