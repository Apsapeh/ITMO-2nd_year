package servlets;

import java.io.IOException;
import java.util.stream.Collectors;

import general.HitRequestParser;
import general.RequestParser;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

@WebServlet("/controller")
public class ControllerServlet extends HttpServlet {
    @Override
    public void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        doPost(req, resp);
    }

    @Override
    public void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String query = req.getReader().lines()
                .collect(Collectors.joining());

        try {
            RequestParser requestParser = new RequestParser(query);

            if (requestParser.isAddCommand()) {
                HitRequestParser hitParser = new HitRequestParser(requestParser);
                req.setAttribute("hitParser", hitParser);

                if (hitParser.containsPointData())
                    req.getRequestDispatcher("/area-check").forward(req, resp);
                else
                    forwardToIndex(req, resp);
            } else if (requestParser.isClearCommand()) {
                req.getRequestDispatcher("/clear").forward(req, resp);
            } else {
                forwardToIndex(req, resp);
            }
        } catch (InternalError e) {
            resp.sendError(500, e.getMessage());
        }        
    }

    private void forwardToIndex(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        req.getRequestDispatcher("index.jsp").forward(req, resp);
    }
}
