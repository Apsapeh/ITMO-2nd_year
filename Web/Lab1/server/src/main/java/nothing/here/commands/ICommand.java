package nothing.here.commands;

import nothing.here.exceptions.EHttpResponseBaseError;

public interface ICommand {
    String execute() throws EHttpResponseBaseError;
}
