import { useEffect, useState } from "react";
import PaperWaper from "../../components/containers/PaperWaper";
import PaperPage from "../../components/misc/PaperPage";
import Cracha from "./components/misc/Cracha";
import { useCrachaActions } from "./stores/crachaStore";


function CrachaPrint(){
  const [list, setList] = useState([]);
  const {getCracha} = useCrachaActions();

  useEffect(()=>{
    if (list.length !== 0) return;
    const params = new URLSearchParams(window.location.search);
    const ids = params.get('ids').split(',').map(val => Number(val));

    if(ids[0] === 0) return;

    const updateList = async() => await Promise.all(
      ids.map((val) => getCracha(val))
    );
    updateList().then((val) => setList([...val]));

    //debug
    //console.log(ids);
  // eslint-disable-next-line react-hooks/exhaustive-deps
  },[]);

  useEffect(()=>{
    //if(list.length !== 0) window.print();
    //debug
    //console.log(list);
  },[list])

  return(
    <PaperWaper className="py-10 flex overflow-scroll grow">
      <div className="flex flex-col items-center grow gap-5">
        {Array.from({length: Math.ceil(list.length / 4)}).map((_, i)=>
          <PaperPage key={i} orientation="landscape">
            <div className="h-full flex flex-row items-center justify-around">

              {Array.from({length: 4}).map((_, j)=>{
                const idx = j + (i * 4);
                const isEmpt = !(idx < list.length);
                return <Cracha key={j} empt={isEmpt} info={list[idx]}/>;
              })}

            </div>
          </PaperPage>
        )}
      </div>
    </PaperWaper>
  );
}

export default CrachaPrint;