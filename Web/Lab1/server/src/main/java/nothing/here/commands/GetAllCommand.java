package nothing.here.commands;

import nothing.here.RequestData;
import nothing.here.ResponseFactory;
import nothing.here.exceptions.EHttpResponseBaseError;

import java.util.List;


public class GetAllCommand implements ICommand {
    List<RequestData> requests;

    public GetAllCommand(List<RequestData> requests) {
        this.requests = requests;
    }

    @Override
    public String execute() throws EHttpResponseBaseError {
        return ResponseFactory.createOkResponseJson(RequestData.listToJson(requests));
    }
}
