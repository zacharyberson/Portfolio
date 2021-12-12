package dev.zberson.servlet;

import java.io.IOException;
import java.io.PrintWriter;
//import java.util.Random;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.gson.*;

import static dev.zberson.exception.CustomException.printe;

import dev.zberson.model.Employee;
import dev.zberson.service.EmployeeService;
import dev.zberson.service.EmployeeServiceImpl;
//import dev.zberson.session.ClientSession;

public class EmployeeWebService {
    private static EmployeeService es = new EmployeeServiceImpl();
    
    public static void checkCredentials(HttpServletRequest request, HttpServletResponse response) throws IOException {
        String username = null;
        String password = null;
        PrintWriter pw = null;

        try {
            pw = response.getWriter();
        } catch (IOException e1) {
            printe("Could not get reponse PrintWriter: " + e1.getMessage());
            return;
        }
        username = request.getParameter("username");
        password = request.getParameter("password");
        System.out.println(username);
        System.out.println(password);
        if(null != username && null != password) {
            Employee test = es.checkCredentials(username, password);
            if(null != test) {
                Gson gson = new Gson();
                gson.toJson(test, pw);
            }
            else {
                pw.append("false");
            }
        }
        else {
            pw.append("false");
        }
        return;
    }
}
