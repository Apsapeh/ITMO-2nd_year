import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable } from 'rxjs';
import { HitCheckGraphRequest, HitCheckRequest, HitResult, HitTableRequest, HitTableResponse } from '../models/hit.model';
import { AuthService } from './auth.service';
import { apiBaseUrl } from '../app.config';

@Injectable({
  providedIn: 'root'
})
export class HitService {
  private apiUrl =  `${apiBaseUrl}/hit`;

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

  getResults(request: HitTableRequest): Observable<HitTableResponse> {
    return this.http.post<HitTableResponse>(`${this.apiUrl}/results`, request, {
      headers: this.getHeaders()
    });
  }

  clearResults(): Observable<void> {
    return this.http.delete<void>(`${this.apiUrl}/results/clear`, {
      headers: this.getHeaders()
    });
  }
}

