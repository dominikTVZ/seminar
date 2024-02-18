from flask import Flask, url_for, redirect, request, make_response, render_template, session, g, jsonify
import jinja2
import MySQLdb
import requests
import plotly.express as px


app = Flask("Prvi program")
app.secret_key = '_de5jRRR83x'

@app.before_request
def before_request_func():
    g.connection = MySQLdb.connect(host = 'localhost', user = 'app', passwd = '1234', db = 'seminar')
    g.cursor = g.connection.cursor()
 
    if request.path == '/login' or  request.path.startswith('/static') or request.path == '/temperatura': 
        return
    if session.get('username') is None:
        return redirect(url_for('login'))
        
@app.after_request
def after_request_func(response):
    g.connection.commit()
    g.connection.close()
    return response

@app.get('/login')
def login():
    response = render_template('login.html',  naslov = 'Stranica za prijavu')
    return response

@app.get('/logout')
def logout():
    session.pop('username')
    return redirect(url_for('login'))

@app.post('/login')
def check():    
    username = request.form.get('username')
    password = request.form.get('password')    
    upit  = render_template('select_user.sql', username = username, password = password)
    odgovor = g.cursor.execute(upit)
    print("Ovo je odgovor:")
    print(odgovor)
    redak = g.cursor.fetchone()
    print("Ovo je redak:")
    print(redak)
    print("ovo su ovlasti:")
    print(redak[5])
    if redak:
        id_korisnika = redak[0]
        ime = redak[1]
        prezime = redak[2]
        id_ovlasti = redak[5]

        print("Ime korisnika:", ime)  # Ispisuje ime korisnika prije postavljanja u sesiju
        print("Prezime korisnika:", prezime)  # Ispisuje prezime korisnika prije postavljanja u sesiju

        session['id'] = id_korisnika
        session['ime'] = ime
        session['prezime'] = prezime
        session['id_ovlasti'] = id_ovlasti
        session['username'] = username
        return redirect(url_for('index'))
        
    else:
        return render_template('login.html', naslov='Stranica za prijavu', poruka='Uneseni su pogresni podaci')

@app.get('/')
def index():
    id_ovlasti = session.get('id_ovlasti')
    id_param = request.args.get('id')
    #username = session.get('username')
    if id_param == '1' or id_param == None:
        upit = render_template('select_temp.sql')
        odgovor = g.cursor.execute(upit)
        podaci = g.cursor.fetchall()   
        print(odgovor)
        print(podaci)
        print("ovo su ovlasti:")
        print(id_ovlasti)   
        response = render_template('index.html', naslov = 'Pocetna stranica', tip = 'Temperatura', username = session.get('username').capitalize(), podaci = podaci, id_ovlasti =id_ovlasti)
        return response, 200

# dodavanje temperature

# Dodavanje i dohvaćanje temperature
@app.route('/temperatura', methods=['GET', 'POST'])
def temperatura():
    global temperatura
    if request.method == 'POST':
        print("Uso sam u POST")        
        response = make_response()
        if request.json.get('temperatura') is not None:
                upit = render_template('dodaj_u_bazu.sql', value = request.json.get('temperatura'))
                g.cursor.execute(upit)
                response.data = 'Uspjesno ste postavili temperaturu'
                response.status_code = 201
        else:
            response.data = 'Niste postavili temperaturu'
            response.status_code = 404
        return response

    if request.method == 'GET':
        print("Uso sam u GET")
        # Dohvaćanje zadnje temperature iz baze podataka
        query = render_template('fetch_latest_temp.sql')
        g.cursor.execute(query)
        temperatura = g.cursor.fetchone()[0]
        print(temperatura)
        print(jsonify({"temperatura": temperatura}))  # Vrijednost temperature je prvi stupac rezultata
        return jsonify({"temperatura": temperatura})



# brisanje temperature
@app.route('/temperatura/<int:id_retka>', methods=['POST'])
def delete(id_retka):
    print(id_retka)
    upit = render_template('delete.sql', id_retka=id_retka)
    g.cursor.execute(upit) 
    return redirect(url_for('index'))

# nova putanja
@app.route('/user')
def check_user_spec():
    id_ovlasti = session.get('id_ovlasti')
    ime = session.get('ime')
    prezime = session.get('prezime')
    upit  = render_template('graf.sql')
    odgovor = g.cursor.execute(upit)
    rows = g.cursor.fetchall()
    print(rows)
    
    # Izrada podataka za graf
    x = [row[0] for row in rows]  # Vrijeme mjerenja
    y = [row[1] for row in rows]  # Temperatura   
    data = {'Vrijeme mjerenja': x, 'Temperatura[C°]': y}
    
    # Izrada grafa
    fig = px.line(data, x='Vrijeme mjerenja', y='Temperatura[C°]', title='Temperature')

    # Spremanje grafa u HTML string
    graph_html = fig.to_html(full_html=False)
    # Renderiranje HTML-a s grafovima

    return render_template('user.html', graph_html=graph_html, id_ovlasti = id_ovlasti)



app.run(host="0.0.0.0", port=80)
