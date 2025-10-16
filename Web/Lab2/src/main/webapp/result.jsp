<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="general.HitData" %>
<%
    HitData hitData = (HitData) request.getAttribute("hitData");
%>
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
                <%= hitData.toHtml() %>
            </tbody>
        </table>
        <a class="back_link" href="/controller"> Назад </a>
    </div>
</body>
</html>