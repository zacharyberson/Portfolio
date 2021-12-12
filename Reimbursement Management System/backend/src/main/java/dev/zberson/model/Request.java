package dev.zberson.model;

import java.util.Arrays;

import dev.zberson.definition.Reason;
import dev.zberson.definition.Status;

public class Request {

    private int status = Status.PENDING;
	private int reason = Reason.OTHER;
	private double amount = 0d;
    private int userId = 0;
	private String username = null;
	private String fullName = null;
    private String timeSubmitted = null;
    private String timeProcessed = null;
    private String note = null;
	private String img = null;
    
	public Request() {
		super();
	}

	public Request(int status) {
	    this(status, 0, 0, 0, null, null, null, null, null);
	}
	
    public Request(int status, String username) {
        this(status, 0, 0, 0, username, null, null, null, null);
    }

    public Request(int status, String username, String timeSubmitted) {
        this(status, 0, 0, 0, username, null, timeSubmitted, null, null);
    }
	
	public Request(int status, int reason, double amount, String username) {
		this(status, reason, amount, 0, username, null, null, null, null);
	}
	
	public Request(int status, int reason, double amount, String username,
	        String note) {
	    
	    this(status, reason, amount, 0, username, null, null, null, note);
	    
	}
	
	public Request(int status, int reason, double amount, int userId,
	        String username, String fullName, String submitTime,
	        String processTime, String note) {
        
	    super();
        this.status = status;
        this.reason = reason;
        this.amount = amount;
        this.userId = userId;
        this.username = username;
        this.fullName = fullName;
        this.timeSubmitted = submitTime;
        this.timeProcessed = processTime;
        this.note = note;
    }

    public Request(int status, int reason, double amount, int userId,
             String username, String fullName, String submitTime,
             String processTime, String note, String img) {
        
         super();
         this.status = status;
         this.reason = reason;
         this.amount = amount;
         this.userId = userId;
         this.username = username;
         this.fullName = fullName;
         this.timeSubmitted = submitTime;
         this.timeProcessed = processTime;
         this.note = note;
         this.img = img;
     }
	
    public int getStatus() {
		return status;
	}
	
	public void setStatus(int status) {
		this.status = status;
	}
	
	public int getReason() {
		return reason;
	}
	
	public void setReason(int reason) {
		this.reason = reason;
	}
	
	
    public int getUserId() {
        return userId;
    }

    
    public void setUserId(int userId) {
        this.userId = userId;
    }

    
    public String getFullName() {
        return fullName;
    }

    
    public void setFullName(String fullName) {
        this.fullName = fullName;
    }

    
    public String getTimeSubmitted() {
        return timeSubmitted;
    }

    
    public void setTimeSubmitted(String timeSubmitted) {
        this.timeSubmitted = timeSubmitted;
    }

    
    public String getTimeProcessed() {
        return timeProcessed;
    }

    
    public void setTimeProcessed(String timeProcessed) {
        this.timeProcessed = timeProcessed;
    }

    public double getAmount() {
		return amount;
	}
	
	public void setAmount(double amount) {
		this.amount = amount;
	}
	
	public String getUsername() {
		return username;
	}
	
	public void setUsername(String username) {
		this.username = username;
	}
	
	public String getNote() {
		return note;
	}
	
	public void setNote(String note) {
		this.note = note;
	}

	public String getSubmitted() {
		return timeSubmitted;
	}

	public void setSubmitted(String timeSql) {
		this.timeSubmitted = timeSql;
	}
	
	public void addImg(String img) {
	    this.img = img;
	}
	
	public String getImg() {
//	    return byteToHex(this.img);
	    return this.img;
	}
	
    @Override
    public String toString() {
        return "Request [status=" + status + ", reason=" + reason + ", amount=" + amount + ", userId=" + userId
                + ", username=" + username + ", fullName=" + fullName + ", timeSubmitted=" + timeSubmitted
                + ", timeProcessed=" + timeProcessed + ", note=" + note + ", img=" + img + "]";
    }
}
