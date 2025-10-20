// Константы
const GRAPH_CENTER = 150;
const GRAPH_SCALE = 100;

const COMMAND_FIELD = "__cmd_type"
const ADD_COMMAND = "add"
const CLEAR_COMMAND = "clear"

function validateInputRange(name, inputId, min, max) {
    const input = document.getElementById(inputId);
    const value = parseFloat(input.value);
    if (isNaN(value) || value < min || value > max) {
        alert(`Значение ${name} должно быть от ${min} до ${max}`);
        input.focus();
        return false;
    }
    return true;
}

function addPointToGraph(xValue, yValue, rValue, hitResult) {
    const graphPoints = document.getElementById('graph-points');
    const scale = GRAPH_SCALE / rValue;
    const svgX = GRAPH_CENTER + (xValue * scale);
    const svgY = GRAPH_CENTER - (yValue * scale);

    // Текст
    const text = document.createElementNS("http://www.w3.org/2000/svg", "text");
    text.setAttribute("x", svgX);
    text.setAttribute("y", svgY - 10);
    text.setAttribute("text-anchor", "middle");
    text.setAttribute("font-size", "12px");
    text.setAttribute("fill", "white");
    text.textContent = `(${(xValue / rValue).toFixed(2)}, ${(yValue / rValue).toFixed(2)})`;

    // Точка
    const circle = document.createElementNS("http://www.w3.org/2000/svg", "circle");
    circle.setAttribute("cx", svgX);
    circle.setAttribute("cy", svgY);
    circle.setAttribute("r", "3");
    circle.setAttribute("class", hitResult === "Попадание" ? "hit-point" : "miss-point");

    graphPoints.appendChild(text);
    graphPoints.appendChild(circle);
}

async function sendRequest(data) {
    try {
        const response = await fetch("/controller", {
            method: "POST",
            body: data,
            headers: {
                "Content-Type": "application/x-www-form-urlencoded",
                accept: "text/html",
            },
        });

        if (!response.ok) {
            const errorText = await response.text();
            alert(`Ошибка ${response.status}: ${errorText}`);
            return null;
        }

        return await response.text();
    } catch (error) {
        alert("Ошибка сети: " + error.message);
        return null;
    }
}

function parseResponse(htmlString) {
    const tempDiv = document.createElement('div');
    tempDiv.innerHTML = htmlString;

    return {
        x: parseFloat(tempDiv.querySelector('#x').textContent),
        y: parseFloat(tempDiv.querySelector('#y').textContent),
        r: parseFloat(tempDiv.querySelector('#r').textContent),
        hit: tempDiv.querySelector('#hit').textContent.trim(),
        result_tr: tempDiv.querySelector('#result-tr')
    };
}

// Валидация формы
function validateForm() {
    return validateInputRange("Y", "ValueY", -3, 3) &&
           validateInputRange("R", "ValueR", 1, 4);
}

// Валидация ввода
function setupInputValidators() {
    const elements = { "ValueY": "", "ValueR": "" };
    const availableChars = "0123456789-.";

    Object.keys(elements).forEach(id => {
        const element = document.getElementById(id);
        if (!element) return;

        element.addEventListener("input", function (event) {
            const inputChar = event.data;

            if (inputChar && !availableChars.includes(inputChar)) {
                this.value = this.value.slice(0, -1);
                return;
            }

            const value = this.value;
            if (!/^-?\d*\.?\d*$/.test(value)) {
                this.value = elements[id] || "";
                return;
            }

            elements[id] = value;
        });

        elements[id] = element.value;
    });
}

async function clearResultsTable() {
    const formData = new FormData();
    formData.append(COMMAND_FIELD, CLEAR_COMMAND);
    const params = new URLSearchParams(formData);

    await sendRequest(params.toString());

    document.getElementById('graph-points').innerHTML = "";
    document.getElementById('resultsBody').innerHTML = "";
}

// Инициализация при загрузке
document.addEventListener("DOMContentLoaded", function () {
    setupInputValidators();

    // Кнопки выбора X
    const xButtons = document.querySelectorAll('.x-button');
    const hiddenXInput = document.getElementById('ValueX');
    const selectedValueText = document.getElementById('selected-value-text');

    xButtons.forEach(button => {
        button.addEventListener('click', function () {
            hiddenXInput.value = this.value;
            selectedValueText.textContent = `Выбранное значение: ${this.value}`;
        });
    });

    // Загрузка существующих точек
    const resultsBody = document.getElementById('resultsBody');
    const graphPoints = document.getElementById('graph-points');

    if (resultsBody && graphPoints) {
        const rows = resultsBody.querySelectorAll('tr');
        rows.forEach(function (row) {
            const cells = row.querySelectorAll('td');
            if (cells.length >= 4) {
                const x = parseFloat(cells[0].textContent);
                const y = parseFloat(cells[1].textContent);
                const r = parseFloat(cells[2].textContent);
                const hit = cells[3].textContent.trim();
                addPointToGraph(x, y, r, hit);
            }
        });
    }
});

// Отправка формы
document.getElementById("main_form").addEventListener("submit", async function (event) {
    event.preventDefault();

    if (!validateForm()) return;

    const formData = new FormData(this);
    formData.append(COMMAND_FIELD, ADD_COMMAND);
    const params = new URLSearchParams(formData);

    let data = await sendRequest(params.toString());
    if (data) {
        document.documentElement.innerHTML = data;
    }
});

// Клик по графику
document.getElementById("graph").addEventListener("click", async function (event) {
    const r = 1;
    const x = event.offsetX;
    const y = event.offsetY;
    const normX = (x - GRAPH_CENTER) / GRAPH_SCALE;
    const normY = (GRAPH_CENTER - y) / GRAPH_SCALE;

    const formData = new FormData();
    formData.append("ValueR", r);
    formData.append("ValueX", normX);
    formData.append("ValueY", normY);
    formData.append(COMMAND_FIELD, ADD_COMMAND);
    const params = new URLSearchParams(formData);

    let data = await sendRequest(params.toString());
    if (data) {
        let params = parseResponse(data);
        addPointToGraph(params.x, params.y, params.r, params.hit);

        const table = document.getElementById("resultsBody");
        table.appendChild(params.result_tr);
    }
});