import { Suspense } from "solid-js";

import { Router } from "@solidjs/router";
import { FileRoutes } from "@solidjs/start/router";

import "./app.css";

export const appStyle = {
  bgCol1: "bg-slate-800",
  bgCol2: "bg-slate-700",
  textCol1: "text-slate-100",
  textCol2: "",
  borderCol1: "border-slate-300",
};

export default function App() {
  return (
    <Router
      root={(props) => (
        <Suspense>{props.children}</Suspense>
      )}>
      <FileRoutes/>
    </Router>
  );
}
