package nothing.here;

import nothing.here.exceptions.EBadResponse;
import nothing.here.exceptions.EInternalError;

import java.text.Format;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class QueryParser {
    static public List<RequestData> parse(String query) throws EBadResponse, EInternalError {
        if (query == null) throw new EInternalError("Query is null");
        if (query.isEmpty()) throw new EBadResponse("GET request has no query");

        long startTime = System.nanoTime();

        HashMap<String, String> queryMap = new java.util.HashMap<>();
        for (String param : query.split("&")) {
            String[] pair = param.split("=", 2);
            if (pair.length > 1) {
                queryMap.put(pair[0], pair[1]);
            } else {
                queryMap.put(pair[0], "on");
            }
        }

        final Pair<String, Float>[] ValueX_Vars = new Pair[]{
                new Pair<>("ValueX_n_2", -2.0f),
                new Pair<>("ValueX_n_1_5", -1.5f),
                new Pair<>("ValueX_n_1", -1.0f),
                new Pair<>("ValueX_n_0_5", -0.5f),
                new Pair<>("ValueX_0", 0f),
                new Pair<>("ValueX_0_5", 0.5f),
                new Pair<>("ValueX_1", 1.0f),
                new Pair<>("ValueX_1_5", 1.5f),
                new Pair<>("ValueX_2", 2.0f)
        };

        List<Float> valuesX  = new ArrayList<>();
        for (Pair<String, Float> x_var : ValueX_Vars) {
            String value = queryMap.get(x_var.first);
            if (value != null && value.equals("on")) {
                valuesX.add(x_var.second);
            }
        }

        if (valuesX.isEmpty()) throw new EBadResponse("No value for X value");


        Float valueY = null;
        String valueY_str = queryMap.get("ValueY");
        try {
            valueY = Float.parseFloat(valueY_str);
        } catch (NumberFormatException | NullPointerException e) {
            throw new EBadResponse("Invalid value for Y value. It should be a real number");
        }

        if (valueY < -3 || valueY > 3) throw new EBadResponse("Invalid value for Y value. It should be in range [-3, 3]");

        Integer valueR = null;
        String valueR_str = queryMap.get("ValueR");
        try {
            valueR = Integer.parseInt(valueR_str);
        } catch (NumberFormatException | NullPointerException e) {
            throw new EBadResponse("Invalid value for R value. It should be an integer");
        }

        if (valueR < 1 || valueR > 5) throw new EBadResponse("Invalid value for R value. It should be in range [1, 5]");


        Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String date = formatter.format(new java.util.Date());

        long endTime = System.nanoTime();
        float elapsedTimeMs = (float)(endTime - startTime) / 1000000.0f;

        List<RequestData> requestDataList = new ArrayList<>();
        for (float x : valuesX)
            requestDataList.add(new RequestData(x, valueY, valueR, date, elapsedTimeMs));
        return requestDataList;
    }
}
