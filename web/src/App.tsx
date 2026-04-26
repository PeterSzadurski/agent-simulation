
import { useState, useRef } from 'react';
import SimulationCanvas from './canvas'
import {  Grid } from '@mui/material';
import { ThemeProvider, createTheme } from '@mui/material/styles';
import CssBaseline from '@mui/material/CssBaseline';
import SimStats from './SimStats';
import SimStatsPanel from './SimStatsPanel';

const darkTheme = createTheme({
  palette: {
    mode: 'dark',
  },
});


function App() {

    const [tick, setTick] = useState(0);
    const [speed, setSpeed] = useState(1);
    const [simStats, setSimStats] = useState<SimStats>(new SimStats());
  return (
    <ThemeProvider theme={darkTheme}>
      <CssBaseline />
      <Grid container spacing={1}>
        <Grid size={4}>
          <SimStatsPanel simStats={simStats}></SimStatsPanel>
        </Grid>
        <Grid size={4}>
          <SimulationCanvas onTick={setTick} onSimStats={setSimStats}/>
        </Grid>
        <Grid size={4}></Grid>
      </Grid>
    </ThemeProvider>
  )
}

export default App
