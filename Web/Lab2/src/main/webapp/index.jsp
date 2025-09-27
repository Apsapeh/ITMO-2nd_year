<%@ page contentType="text/html;charset=UTF-8" language="java" %>
    <%@ page import="general.HitData, java.util.concurrent.LinkedBlockingDeque, java.util.Deque, java.util.Locale" %>
        <!DOCTYPE html>
        <html lang="ru">

        <head>
            <meta charset="UTF-8" />
            <title>Web Lab 2</title>
            <link rel="stylesheet" type="text/css" href="index.css" />
            <script src="https://code.jquery.com/jquery-3.7.1.js"
                integrity="sha256-eKhayi8LEQwp4NKxN+CfCh+3qOVUtJn3QNZ0TciWLP4=" crossorigin="anonymous"></script>
            <link href="https://fonts.googleapis.com/css2?family=Comfortaa:wght@300..700&display=swap" rel="stylesheet">

        </head>

        <body>
            <header id="main-header" class="glass header-style">
                <h1>Тимофей Тарасович Кириченко, P3231, Вариант 2219</h1>
            </header>

            <!-- <br /> -->

            <!-- Основная табличная верстка -->
            <table class="layout glass">
                <tr>
                    <td class="graph-sidebar">
                        <div class="glass" style="padding: 20px;">
                            <svg id="graph" height="300" width="300" xmlns="http://www.w3.org/2000/svg">
                                <!-- Оси со стрелками -->
                                <line class="axis-line" x1="0" x2="300" y1="150" y2="150" />
                                <line class="axis-line" x1="150" x2="150" y1="0" y2="300" />
                                <polygon class="axis-arrow" points="150,0 144,15 156,15" />
                                <polygon class="axis-arrow" points="300,150 285,156 285,144" />

                                <!-- Засечки -->
                                <line class="axis-tick" x1="200" x2="200" y1="155" y2="145" />
                                <line class="axis-tick" x1="250" x2="250" y1="155" y2="145" />
                                <line class="axis-tick" x1="50" x2="50" y1="155" y2="145" />
                                <line class="axis-tick" x1="100" x2="100" y1="155" y2="145" />
                                <line class="axis-tick" x1="145" x2="155" y1="100" y2="100" />
                                <line class="axis-tick" x1="145" x2="155" y1="50" y2="50" />
                                <line class="axis-tick" x1="145" x2="155" y1="200" y2="200" />
                                <line class="axis-tick" x1="145" x2="155" y1="250" y2="250" />

                                <!-- Подписи -->
                                <text class="axis-label" x="195" y="140">R/2</text>
                                <text class="axis-label" x="248" y="140">R</text>
                                <text class="axis-label" x="40" y="140">-R</text>
                                <text class="axis-label" x="90" y="140">-R/2</text>
                                <text class="axis-label" x="160" y="105">R/2</text>
                                <text class="axis-label" x="160" y="55">R</text>
                                <text class="axis-label" x="160" y="205">-R/2</text>
                                <text class="axis-label" x="160" y="255">-R</text>
                                <text class="axis-title" x="160" y="20">Y</text>
                                <text class="axis-title" x="280" y="140">X</text>

                                <!-- Фигуры -->
                                <rect class="shape-rectangle" x="50" y="100" width="100" height="50" />

                                <polygon class="shape-triangle" points="150,100 150,150 200,150" />

                                <path class="shape-semicircle" d="M 200 150 A 50 50, 0, 0, 1, 150, 200 L 150 150 Z" />

                                <!-- Центр -->
                                <circle class="center-point" cx="150" cy="150" r="3" />

                                <g id="graph-points">
                                    <!-- <text x="250" y="240" text-anchor="middle" font-size="14px">hello</text> -->
                                    <!-- <circle class="miss-point" cx="250" cy="250" r="3"></circle> -->
                                </g>


                            </svg>
                        </div>
                    </td>

                    <td style="vertical-align: top;">
                        <form id="main_form">
                            <table class="form-vertical">
                                <tr>
                                    <td class="form-cell glass">
                                        <label class="form-label">X</label>
                                        <p id="selected-value-text">Выбранное значение: 0.0</p>
                                        <div class="checkbox-container" id="ValueXContainer">
                                            <input class="x-button glass" type="button" value="-2.0" />
                                            <input class="x-button glass" type="button" value="-1.5" />
                                            <input class="x-button glass" type="button" value="-1.0" />
                                            <input class="x-button glass" type="button" value="-0.5" />
                                            <input class="x-button glass" type="button" value="0.0" />
                                            <input class="x-button glass" type="button" value="0.5" />
                                            <input class="x-button glass" type="button" value="1.0" />
                                            <input class="x-button glass" type="button" value="1.5" />
                                            <input class="x-button glass" type="button" value="2.0" />
                                        </div>
                                        <input type="hidden" id="ValueX" name="ValueX" value="0.0" />
                                    </td>
                                </tr>
                                <tr>
                                    <td class="form-cell glass">
                                        <label class="form-label" for="ValueY">Y</label>
                                        <input class="input-text glass" type="text" id="ValueY" name="ValueY"
                                            maxlength="6" placeholder="Введите число от -3 до 3" />
                                    </td>
                                </tr>
                                <tr>
                                    <td class="form-cell glass">
                                        <label class="form-label" for="ValueR">R</label>
                                        <input class="input-text glass" type="text" id="ValueR" name="ValueR"
                                            maxlength="6" placeholder="Введите число от 1 до 4" />
                                    </td>
                                </tr>
                                <tr>
                                    <td class="form-cell">
                                        <input class="input-button glass" type="submit" value="Отправить" />
                                    </td>
                                </tr>
                            </table>
                        </form>
                    </td>
                </tr>

                <tr>
                    <td colspan="2" class="footer glass">
                        <div class="results-header">
                            <label class="form-label">Результаты</label>
                            <!-- <button onclick="clearResultsTable()" class="clear-button glass">
                            Очистить
                        </button> -->
                        </div>

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
                                <% Deque<HitData> collection = (Deque<HitData>)
                                        request.getServletContext().getAttribute("globalHitHistoryDeque");

                                        // HitResult test = new HitResult();

                                        if (collection != null) {
                                        for (HitData data : collection) {
                                        %>
                                        <%= data.toHtml() %>
                                            <% } } %>
                            </tbody>
                        </table>
                    </td>
                </tr>
            </table>

            <script src="main.js"></script>
        </body>

        </html>