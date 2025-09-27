package exceptions;

public class EBadResponse extends EHttpResponseBaseError {
    public EBadResponse(String message) {
        super(message);
    }
}
