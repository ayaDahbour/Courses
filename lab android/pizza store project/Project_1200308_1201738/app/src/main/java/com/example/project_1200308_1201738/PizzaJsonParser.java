package com.example.project_1200308_1201738;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import java.util.ArrayList;
import java.util.List;
public class PizzaJsonParser {
    public static List<Pizza> getObjectFromJson(String json) {
        List<Pizza> pizzaes;
        try {
            JSONObject jsonObject = new JSONObject(json);
            if (jsonObject.has("types")) {
                JSONArray jsonArray = jsonObject.getJSONArray("types");
                pizzaes = new ArrayList<>();
                for (int i = 0; i < jsonArray.length(); i++) {
                    String type = jsonArray.getString(i);
                    Pizza pizza = new Pizza();
                    pizza.setId(i);
                    pizza.setType(type);
                    pizzaes.add(pizza);
                }
            } else {
                // Handle single object case
                pizzaes = new ArrayList<>();
                Pizza pizza = new Pizza();
                pizza.setId(jsonObject.getInt("id"));
                pizza.setType(jsonObject.getString("name"));
                pizzaes.add(pizza);
            }
        } catch (JSONException e) {
            e.printStackTrace();
            return null;
        }
        return pizzaes;
    }
}
