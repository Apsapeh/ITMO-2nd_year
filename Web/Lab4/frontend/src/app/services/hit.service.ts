import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable } from 'rxjs';
import { HitCheckGraphRequest, HitCheckRequest, HitResult } from '../models/hit.model';
import { AuthService } from './auth.service';

@Injectable({
  providedIn: 'root'
})
export class HitService {
  private apiUrl = 'http://localhost:28080/api/hit';

  constructor(private http: HttpClient, private authService: AuthService) {}

  private getHeaders(): HttpHeaders {
    const token = this.authService.getToken();
    return new HttpHeaders({
      'Authorization': `Bearer ${token}`,
      'Content-Type': 'application/json'
    });
  }

  checkHit(request: HitCheckRequest): Observable<HitResult> {
    return this.http.post<HitResult>(`${this.apiUrl}/check`, request, {
      headers: this.getHeaders()
    });
  }

  checkHitGraph(request: HitCheckGraphRequest): Observable<HitResult> {
    return this.http.post<HitResult>(`${this.apiUrl}/checkGraph`, request, {
      headers: this.getHeaders()
    });
  }

  getResults(): Observable<HitResult[]> {
    return this.http.get<HitResult[]>(`${this.apiUrl}/results`, {
      headers: this.getHeaders()
    });
  }

  clearResults(): Observable<void> {
    return this.http.delete<void>(`${this.apiUrl}/results/clear`, {
      headers: this.getHeaders()
    });
  }
}

