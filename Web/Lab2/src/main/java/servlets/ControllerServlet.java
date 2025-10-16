package servlets;

import java.io.IOException;
import java.util.stream.Collectors;

import general.QueryParser;
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
            QueryParser parser = new QueryParser(query);
            req.setAttribute("queryParser", parser);

            if (parser.containsPointData())
                req.getRequestDispatcher("/area-check").forward(req, resp);
            else
                req.getRequestDispatcher("index.jsp").forward(req, resp);
        } catch (InternalError e) {
            resp.sendError(500, e.getMessage());
        }        
    }
}
