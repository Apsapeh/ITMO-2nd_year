package servlets;

import java.io.IOException;
import java.util.concurrent.LinkedBlockingDeque;

import general.HitData;
import general.QueryParser;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import exceptions.*;


@WebServlet("/area-check")
public class AreaCheckServlet extends HttpServlet {

    @Override
    public void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        req.setCharacterEncoding("UTF-8");

        try {
            // read request data
            Object parserObject = req.getAttribute("queryParser");
            assert (parserObject.getClass() == QueryParser.class);
            QueryParser parser = (QueryParser)parserObject;

            HitData hitData = parser.parseToHitData();

            // save request data to history
            Object contextObject = getServletContext().getAttribute("globalHitHistoryDeque");
            assert (contextObject.getClass() == LinkedBlockingDeque.class);
            @SuppressWarnings("unchecked")
            LinkedBlockingDeque<HitData> hitResults = (LinkedBlockingDeque<HitData>)contextObject;

            hitResults.add(hitData);

            // передаем данные в JSP
            req.setAttribute("hitData", hitData);
            req.getRequestDispatcher("/result.jsp").forward(req, resp);
        } catch (EBadResponse e) {
            resp.sendError(400, e.getMessage());
            return;
        }
    }
}
