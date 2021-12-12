package dev.zberson.servlet;

import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import static dev.zberson.exception.CustomException.printe;

public class MasterServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    public MasterServlet() {
        super();
    }
    
    /**
     * retrieve requests + doPost() requests
     */
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String uri = request.getRequestURI();
        
        switch(uri) {
            case "/Project1/login.do":
                System.out.println("Calling employee web service");
                EmployeeWebService.checkCredentials(request, response);
                break;
            case "/Project1/request.do":
                System.out.println("Calling request fetching web service");
                RequestWebService.process(request, response);
                break;
            default:
                printe("Unrecognized URI: " + uri);
        }
        
    }

    /**
     * either login, submit request, or approve/deny request
     */
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        doGet(request, response);
    }
}
