import dash
from dash.dependencies import Output, Event
import dash_core_components as dcc
import dash_html_components as html
import plotly
import random
import plotly.graph_objs as go
import pandas as pd

app = dash.Dash()

app.layout = html.Div(
    [    html.H2('Room Temperature'),
        dcc.Graph(id='live-graph', animate=True),
        dcc.Interval(id='graph-update', interval=5000)
    ]
)

@app.callback(Output('live-graph', 'figure'), events = [Event('graph-update', 'interval')])
def update_graph():
    df = pd.read_csv('temps.csv', names=['timestamp', 'temp'])
    df = df[(df['temp'] < 40) & (df['temp'] > 0)]
    last_n = 720 * 3
    X_text = pd.to_datetime(df['timestamp'].tail(last_n), unit='s', utc=True).apply(lambda t: convert_timestamp(t)).values
    X = df['timestamp'].tail(last_n).values
    df['temp_smoothed'] = df['temp'].ewm(alpha=0.03).mean()
    Y = df['temp_smoothed'].tail(last_n).values
    data = go.Scatter(x=X, y=Y, name='Temp C', mode='lines', line=dict(shape='spline'))
    f = lambda m, n: [i*n//m + n//(2*m) for i in range(m)]
    indices = f(6, len(X))
    tick_vals = X[indices]
    tick_texts = X_text[indices]
    return {'data': [data], 'layout': go.Layout(xaxis = dict(range=[min(X), max(X)], tickvals=tick_vals, ticktext=tick_texts),
                                                 yaxis = dict(range=[min(Y), max(Y)])) }
def convert_timestamp(t):
    return str((t.hour + 2)%24).zfill(2) + ':' + str(t.minute).zfill(2) + ':' + str(t.second).zfill(2)
if __name__ == '__main__':
    app.run_server(host='0.0.0.0')
