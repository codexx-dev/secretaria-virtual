import { createBrowserRouter, RouterProvider } from 'react-router-dom'
import { StrictMode } from 'react'
import { hydrateRoot } from 'react-dom/client'
import { BASENAME, routes } from './routes';
import './index.css'


hydrateRoot(document.getElementById('root'),
  <StrictMode>
    <RouterProvider router={
      createBrowserRouter(routes, {basename:BASENAME})
    } />
  </StrictMode>
);
