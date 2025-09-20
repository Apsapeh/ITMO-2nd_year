// Валидация в реальном времени
document.getElementById("ValueY").addEventListener("input", function (e) {
    const value = e.target.value;
    // Разрешаем только числа, минус и точку
    if (!/^[-]?[0-9]*[.]?[0-9]*$/.test(value)) {
        e.target.value = value.slice(0, -1);
    }
});

// Загрузка предыдущих результатов при загрузке страницы
document.addEventListener("DOMContentLoaded", async function () {
    clearResultsTable();

    const url = "/fcgi-bin/server.jar/getAll";
    var data = await sendRequest(url);

    if (data) {
        addResultToTable(data);
    }
});

// Основной обработчик формы
document.getElementById("main_form").addEventListener("submit", async function (event) {
    event.preventDefault();

    // Валидация перед отправкой
    if (!validateForm()) {
        return;
    }

    const formData = new FormData(this);
    const params = new URLSearchParams(formData);
    const url = `/fcgi-bin/server.jar/add?${params.toString()}`;

    var data = await sendRequest(url);

    if (data) {
        addResultToTable(data);
    }
});

async function sendRequest(url) {
    try {
        const response = await fetch(url, { method: "GET" });

        if (!response.ok) {
            const errorText = await response.text();
            alert(`Ошибка ${response.status}: ${errorText}`);
            return null;
        }

        return await response.json();
    } catch (error) {
        alert("Ошибка сети: " + error.message);
        return null;
    }
}

function validateForm() {
    // Валидация Y
    const yInput = document.getElementById("ValueY");
    const yValue = parseFloat(yInput.value);

    if (isNaN(yValue) || yValue < -3 || yValue > 3) {
        alert("Y должен быть числом от -3 до 3");
        yInput.focus();
        return false;
    }
    return true;
}

function addResultToTable(resultData) {
    const tbody = document.getElementById("resultsBody");

    for (var r of resultData) {
        const row = document.createElement("tr");
        row.innerHTML = `
        <td>${r.x.toFixed(2)}</td>
        <td>${r.y.toFixed(2)}</td>
        <td>${r.r}</td>
        <td class="${r.hit ? "hit" : "miss"}">${r.hit ? "Попадание" : "Промах"}</td>
        <td>${r.date}</td>
        <td>${r.time} мс</td>
    `;

        tbody.appendChild(row);
    }

    //row.scrollIntoView({ behavior: "smooth" });
}

function clearResultsTable() {
    document.getElementById("resultsBody").innerHTML = "";
}
