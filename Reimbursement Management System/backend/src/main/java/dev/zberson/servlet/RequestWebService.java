package dev.zberson.servlet;

import static dev.zberson.exception.CustomException.printe;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.gson.Gson;

import dev.zberson.definition.Status;
import dev.zberson.model.Request;
import dev.zberson.service.RequestService;
import dev.zberson.service.RequestServiceImpl;

public class RequestWebService {
    private static RequestService rs = new RequestServiceImpl();

    public static void process(HttpServletRequest request, HttpServletResponse response) {
        String type = request.getParameter("type");
        
        System.out.println(type);
        if(null != type) {
            switch(type) {
                case "get":
                    getRequests(request, response);
                    break;
                case "submit":
                    submitRequest(request, response);
                    break;
                case "process":
                    processRequest(request,response);
                    break;
                default:
                    System.out.println("Invalid Parameter Type");
                    return;
            }
        } else {
            System.out.println("No Parameter Found");
        }
    }

    private static void getRequests(HttpServletRequest request, HttpServletResponse response) {
        int status;
        String username = request.getParameter("username");
        List<Request> requests;

        try {
            status = Integer.parseInt(request.getParameter("status"));
        } catch (NumberFormatException e) {
            System.out.println(e.getMessage());
            respond(response, false);
            return;
        }
        
        requests = rs.getRequests(status, username);
        if(null != requests)
            respond(response, requests);
        else
            respond(response, false);
    }

    private static void submitRequest(HttpServletRequest request, HttpServletResponse response) {
        System.out.println("got the request: " + request);
        Gson g = new Gson();
        BufferedReader br;
        try {
            br = request.getReader();
        } catch (IOException e1) {
            System.out.println("failed to get bufferedreader from request");
            e1.printStackTrace();
            return;
        }
        StringBuilder sb = new StringBuilder();
        char[] ca = new char[16];
        Request r;
        String username;
        String note; 
        int reason;
        double amount;
        String img;
        int len;
        
        try {
            len = br.read(ca, 0, 16);
            while(-1 != len) {
                sb.append(ca, 0, len);
                len = br.read(ca, 0, 16);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println("json: " + sb.toString());
        r = g.fromJson(sb.toString(), Request.class);
        username = r.getUsername();
        note = r.getNote();
        reason = r.getReason();
        amount = r.getAmount(); 
        img = r.getImg();
        if(null != img)
            respond(response, rs.submitRequest(reason, amount, username, note, img));
        else
            respond(response, rs.submitRequest(reason, amount, username, note));        
    }
    
    private static void processRequest(HttpServletRequest request, HttpServletResponse response) {
        int status;
        Gson g = new Gson();
        Request r = g.fromJson(request.getParameter("reimbursement"), Request.class);
        String username = r.getUsername();
        String timeSubmitted = r.getTimeSubmitted();
        System.out.println("username " + username);
        System.out.println("timeSubmitted " + timeSubmitted);
        System.out.println("Request " + r);
        try {
            status = Integer.parseInt(request.getParameter("status"));
            if (status != Status.APPROVED && status != Status.DENIED)
                throw new NumberFormatException("invalid status");
        } catch (NumberFormatException ex) {
            System.out.println(ex.getMessage());
            respond(response, false);
            return;
        }
        if(status == Status.APPROVED && rs.approveRequest(username, timeSubmitted)) {
            r.setStatus(Status.APPROVED);
            respond(response, r);
        }
        else if(status == Status.DENIED && rs.denyRequest(username, timeSubmitted)) {
            r.setStatus(Status.DENIED);
            respond(response, r);
        }
        
    }

    private static void respond(HttpServletResponse response, Request r) {
        PrintWriter pw = null;
        Gson g = new Gson();
        
        try {
            pw = response.getWriter();
        } catch (IOException e1) {
            printe("Could not get reponse PrintWriter: " + e1.getMessage());
            return;
        }
        
        g.toJson(r,pw);
    }
    
    private static void respond(HttpServletResponse response, List<Request> requests) {
        PrintWriter pw = null;
        Gson g = new Gson();
        
        try {
            pw = response.getWriter();
        } catch (IOException e1) {
            printe("Could not get reponse PrintWriter: " + e1.getMessage());
            return;
        }
        
        g.toJson(requests,pw);
    }
    
    private static void respond(HttpServletResponse response, boolean f) {
        PrintWriter pw = null;
        
        try {
            pw = response.getWriter();
        } catch (IOException e1) {
            printe("Could not get reponse PrintWriter: " + e1.getMessage());
            return;
        }
        
        pw.append(Boolean.toString(f));
    }
}

