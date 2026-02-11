import { create } from "zustand";
import { getDB } from "../data/appDB";

export const createUserDataStore = (storeName) => create((set)=> ({
  list: [],

  updateList: async(idxName, query)=>{
    const db = await getDB();
    let result;
    if(idxName && query) result = await db.getAllFromIndex(storeName, idxName, query);
    else result = await db.getAll(storeName);
    
    //debug
    //console.log(result);
    
    set(()=>({
      list: result
    }));
  },

  addItem: async(info)=>{
    const db = await getDB();
    const key = await db.put(storeName, info);
    
    set((state)=>({
      list: [...state.list, {...info, id: key}]
    }));
  },

  removeItem: async(key)=>{
    const db = await getDB();
    await db.delete(storeName, key);

    set((state)=>({
      list: state.list.filter((item)=> item.id !== key)
    }));
  },

  getItem: async(key)=>{
    const db = await getDB();
    return await db.get(storeName, key);
  },

  updateItem: async(info)=>{
    const db = await getDB();
    return await db.put(storeName, info);
  }
}));