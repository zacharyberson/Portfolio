package dev.zberson.session;

public class ClientSession {
    private String username = null;
    private boolean isManager = false;
    private String pageUrl = null;
    
    public ClientSession() {
        super();
    }

    public ClientSession(String username, boolean isManager, String pageUrl) {
        super();
        this.username = username;
        this.isManager = isManager;
        this.pageUrl = pageUrl;
    }
    
    public String getUsername() {
        return username;
    }

    
    public void setUsername(String username) {
        this.username = username;
    }

    
    public boolean isManager() {
        return isManager;
    }

    
    public void setManager(boolean isManager) {
        this.isManager = isManager;
    }

    
    public String getPageUrl() {
        return pageUrl;
    }

    
    public void setPageUrl(String pageUrl) {
        this.pageUrl = pageUrl;
    }

    @Override
    public String toString() {
        return "ClientSession [username=" + username + ", isManager=" + isManager + ", pageUrl=" + pageUrl + "]";
    } 
}
