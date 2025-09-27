package servlets;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Objects;
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

    public static final String DEFAULT_TYPE = "text/html";


    //after redirect, http method should be GET
    @Override
    public void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        req.setCharacterEncoding("UTF-8");

        // check Accept
        String accept = Objects.requireNonNullElse(req.getHeader("Accept"), DEFAULT_TYPE) ;
        
        if ("*/*".equals(accept))
            accept = DEFAULT_TYPE;

        if (!accept.contains(DEFAULT_TYPE)) {
            resp.sendError(400, "Accept can only be " + DEFAULT_TYPE);
            return;
        }

        // check Content-Type
        String contentType = Objects.requireNonNullElse(req.getHeader("Content-Type"), DEFAULT_TYPE);
        if (!"application/x-www-form-urlencoded".equals(contentType)) {
            resp.sendError(400, "Content-Type can only be application/x-www-form-urlencoded");
            return;
        }

        // read request data
        String reqData = req.getAttribute("query").toString();

        try {
            HitData hitData = QueryParser.parse(reqData);

            Object contextObject = getServletContext().getAttribute("globalHitHistoryDeque");
            assert (contextObject.getClass() == LinkedBlockingDeque.class);
            @SuppressWarnings("unchecked")
            LinkedBlockingDeque<HitData> hitResults = (LinkedBlockingDeque<HitData>)contextObject;
            hitResults.add(hitData);

            resp.setCharacterEncoding("UTF-8");
            resp.setContentType("text/html; charset=UTF-8");

            try (PrintWriter writer =  resp.getWriter()) {
                writer.print(generateHtml(hitData));
            }
        } catch (EBadResponse e) {
            resp.sendError(400, e.getMessage());
            return;
        } catch (EInternalError e) {
            resp.sendError(500, e.getMessage());
            return;
        }
    }


    private String generateHtml(HitData data) {
        return """
                <!DOCTYPE html>
                <html lang="ru">

                <head>
                    <meta charset="UTF-8" />
                    <title>Web Lab 1</title>
                    <link rel="stylesheet" href="index.css" />
                    <link href="https://fonts.googleapis.com/css2?family=Comfortaa:wght@300..700&display=swap" rel="stylesheet">
                </head>

                <body>
                    <header id="main-header" class="glass header-style">
                        <h1>Результат проверки</h1>
                    </header>

                    <br />

                    <div class="footer glass">
                        <table id="resultsTable" class="result-table">
                            <thead>
                                <tr>
                                    <th class="result-header">X</th>
                                    <th class="result-header">Y</th>
                                    <th class="result-header">R</th>
                                    <th class="result-header">Результат</th>
                                    <th class="result-header">Дата</th>
                                    <th class="result-header">Время исп.</th>
                                </tr>
                            </thead>
                            <tbody id="resultsBody">
                                %s
                            </tbody>
                        </table>
                        <a class="back_link" href="/controller"> Назад </a>
                        </td>

                </body>

                </html>
                """.formatted(data.toHtml());
    }
}
