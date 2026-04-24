import streamlit as st
import pandas as pd
import urllib.request
import plotly.express as px

PROVINCES = {1: 'Вінницька', 2: 'Волинська', 3: 'Дніпропетровська', 4: 'Донецька', 
    5: 'Житомирська', 6: 'Закарпатська', 7: 'Запорізька', 8: 'Івано-Франківська', 
    9: 'Київська', 10: 'Кіровоградська', 11: 'Луганська', 12: 'Львівська', 
    13: 'Миколаївська', 14: 'Одеська', 15: 'Полтавська', 16: 'Рівненська', 
    17: 'Сумська', 18: 'Тернопільська', 19: 'Харківська', 20: 'Херсонська', 
    21: 'Хмельницька', 22: 'Черкаська', 23: 'Чернівецька', 24: 'Чернігівська', 
    25: 'Республіка Крим'}

st.set_page_config(page_title="Data", layout="wide")

@st.cache_data(show_spinner="Завантаження даних")
def load_data():
    all_data = []
    for prov_id, prov_name in PROVINCES.items():
        url = f"https://www.star.nesdis.noaa.gov/smcd/emb/vci/VH/get_TS_admin.php?country=UKR&provinceID={prov_id}&year1=1981&year2=2026&type=Mean"
        try:
            req = urllib.request.urlopen(url)
            text = req.read().decode('utf-8')
            
            text = text.replace("<br>", "").replace("<tt>", "").replace("</tt>", "").replace("<pre>", "").replace("</pre>", "")
            lines = text.split('\n')
            
            clean_data = []
            for line in lines:
                if not line.startswith("<") and not line.startswith(" ") and len(line.strip()) > 0:
                    parts = [p.strip() for p in line.split(',') if p.strip()]
                    if len(parts) >= 7 and parts[0] != 'year':
                        try:
                            clean_data.append({
                                'Year': int(parts[0]),
                                'Week': int(parts[1]),
                                'SMN': float(parts[2]),
                                'SMT': float(parts[3]),
                                'VCI': float(parts[4]),
                                'TCI': float(parts[5]),
                                'VHI': float(parts[6]),
                                'Province': prov_name
                            })
                        except ValueError:
                            continue
            
            all_data.extend(clean_data)
        except Exception as e:
            continue

    df = pd.DataFrame(all_data)
    df = df[df['VHI'] >= 0]
    return df

df = load_data()

def reset_filters():
    st.session_state.index_sel = "VHI"
    st.session_state.region_sel = "Київська"
    st.session_state.year_slider = (1981, 2026)
    st.session_state.week_slider = (1, 52)
    st.session_state.sort_asc = False
    st.session_state.sort_desc = False

if 'index_sel' not in st.session_state:
    reset_filters()

col1, col2 = st.columns([1, 3])

with col1:
    st.header("Налаштування фільтрів")
    
    st.button("Скинути фільтри", on_click=reset_filters, type="primary")
    
    index_options = ["VCI", "TCI", "VHI"]
    index_sel = st.selectbox("Оберіть індекс:", index_options, key="index_sel")
    
    region_options = list(PROVINCES.values())
    region_sel = st.selectbox("Оберіть область:", region_options, key="region_sel")
    
    year_sel = st.slider("Оберіть інтервал років:", min_value=1981, max_value=2026, key="year_slider")
    
    week_sel = st.slider("Оберіть інтервал тижнів:", min_value=1, max_value=52, key="week_slider")
    
    st.markdown("Сортування")
    sort_asc = st.checkbox("За зростанням", key="sort_asc")
    sort_desc = st.checkbox("За спаднням", key="sort_desc")

df_filtered_time = df[(df['Year'] >= year_sel[0]) & (df['Year'] <= year_sel[1]) & 
                      (df['Week'] >= week_sel[0]) & (df['Week'] <= week_sel[1])]

df_region = df_filtered_time[df_filtered_time['Province'] == region_sel].copy()

df_region['Year_Week'] = df_region['Year'].astype(str) + " - тиждень " + df_region['Week'].astype(str)

if sort_asc and sort_desc:
    st.warning(" Ви увімкнули обидва сортування одночасно, відображенння у хронологічному порядку")
elif sort_asc:
    df_region = df_region.sort_values(by=index_sel, ascending=True)
elif sort_desc:
    df_region = df_region.sort_values(by=index_sel, ascending=False)


with col2:
    tab1, tab2, tab3 = st.tabs(["Таблиця даних", "Графік часового ряду", "Порівняння областей"])
    
    with tab1:
        st.subheader(f"Дані {index_sel} для області: {region_sel}")
        st.dataframe(df_region[['Year', 'Week', index_sel]], use_container_width=True)
        
    with tab2:
        st.subheader(f"Динаміка {index_sel} ({year_sel[0]}-{year_sel[1]})")
        
        if sort_asc != sort_desc:
            fig1 = px.bar(df_region, x='Year_Week', y=index_sel, 
                          title=f"Відсортовані значення {index_sel}",
                          color=index_sel, color_continuous_scale="Viridis")
        else:
            fig1 = px.line(df_region, x='Year_Week', y=index_sel, 
                           title=f"Хронологічний графік {index_sel}")
            fig1.update_traces(mode='lines+markers')
            
        fig1.update_layout(xaxis_title="Рік - тиждень", yaxis_title=index_sel)
        st.plotly_chart(fig1, use_container_width=True)
        
    with tab3:
        st.subheader(f"Порівняння середнього {index_sel} по всіх областях за обраний період")
        
        df_comparison = df_filtered_time.groupby('Province')[index_sel].mean().reset_index()
        df_comparison = df_comparison.sort_values(by=index_sel, ascending=False)
        
        df_comparison['Колір'] = df_comparison['Province'].apply(lambda x: 'Обрана область' if x == region_sel else 'Інші області')
        
        fig2 = px.bar(df_comparison, x='Province', y=index_sel, color='Колір',
                      color_discrete_map={'Обрана область': 'crimson', 'Інші області': 'lightslategray'},
                      title=f"Середній показник {index_sel} ({year_sel[0]}-{year_sel[1]}, тижні {week_sel[0]}-{week_sel[1]})")
        
        fig2.update_layout(xaxis_title="Область", yaxis_title=f"Середнє {index_sel}", xaxis={'categoryorder':'total descending'})
        st.plotly_chart(fig2, use_container_width=True)